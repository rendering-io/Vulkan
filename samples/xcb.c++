#include <vk/vk.h>
#include <cassert>
#include <iostream>
#include <memory>
#include "io.h"

bool handle_events(xcb_connection_t* connection, xcb_atom_t wm_delete_window) {
  bool run = true;
  xcb_generic_event_t *e = nullptr;
  while (run && (e = xcb_poll_for_event(connection))) {

    switch (e->response_type & ~0x80) {
    case XCB_CLIENT_MESSAGE: {
      auto client_msg = reinterpret_cast<xcb_client_message_event_t*>(e);
      if (client_msg->data.data32[0] == wm_delete_window) {
        run = false;
      }
      break;
    }
    }

    free(e);
  }

  return run;
}

auto choose_swapchain_format(const vk::physical_device &device, vk::surface surface) {
  auto formats = device.surface_formats(surface);
  assert(formats.size() > 0);
  return formats[0];
}

auto make_graphics_pipeline(vk::device device) {
  // Load the vertex shader module.
  auto vertex_spirv = load_shader("triangle.vert.spv");
  vk::shader_module vertex_shader{device, vertex_spirv.data(), 
                                  vertex_spirv.size() * sizeof(uint32_t)};

  // Load the fragment shader module.
  auto fragment_spirv = load_shader("triangle.frag.spv");
  vk::shader_module fragment_shader{device, fragment_spirv.data(), 
                                    fragment_spirv.size() * sizeof(uint32_t)};

  // Define a single subpass.
  vk::attachment_reference colour_references{0, vk::image_layout::color_attachment};
  vk::subpass_description subpass{nullptr, 0, &colour_references, 1, nullptr, nullptr, nullptr, 0};

  // Describe the attachments for the render pass.
  vk::attachment_description attachment{
    vk::texel_format::r8g8b8a8_unorm,
    vk::attachment_description::load_operation::dont_care,
    vk::attachment_description::store_operation::store,
    vk::attachment_description::load_operation::dont_care,
    vk::attachment_description::store_operation::store,
    vk::image_layout::undefined,
    vk::image_layout::present_source
  };

  // Create a render pass.
  vk::render_pass render_pass{device, &attachment, 1, &subpass, 1, nullptr, 0};

  // Describe the pipeline layout.
  vk::pipeline_layout layout{device, nullptr, 0};

  // Describe the shader stages.
  std::array<vk::pipeline_shader, 2> stages{
    vk::pipeline_shader{vk::pipeline_shader::shader_stage::vertex, 
                        vertex_shader, "main"}, 
    vk::pipeline_shader{vk::pipeline_shader::shader_stage::fragment,
                        fragment_shader, "main"}
  };

  // Describe the vertex bindings.
  vk::vertex_input_state vertex_input_state;

  // Configure the input assembler.
  vk::input_assembly_state assembly_state;
  assembly_state.primitive_restart_enabled = true;
  assembly_state.topology = vk::input_assembly_state::primitive_topology::point_list;

  // Configure the viewports.
  vk::viewport viewport;
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = 256.0f;
  viewport.height = 256.0f;
  viewport.min_depth = 0.0f;
  viewport.max_depth = 1.0f;

  vk::rect<2> scissor;
  scissor.offset.x = 0;
  scissor.offset.y = 0;
  scissor.extent.width = 256;
  scissor.extent.height = 256;
  vk::viewport_state viewport_state{&viewport, &scissor, 1};
  
  // Configure the rasterizer.
  vk::rasterization_state raster_state;
  raster_state.depth_clamp_enabled = false;
  raster_state.rasterizer_discard_enabled = true;

  // Create the actual pipeline.
  return vk::graphics_pipeline{device, stages.data(), uint32_t(stages.size()),
                               vertex_input_state, assembly_state,
                               viewport_state, raster_state, 
                               layout, render_pass};
}

int main(int argc, char **argv) {
  // Open an xcb connection and get the first screen.
  auto connection = xcb_connect(nullptr, nullptr);
  auto screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

  // Create a window.
  auto window = xcb_generate_id(connection);
  xcb_create_window(connection, XCB_COPY_FROM_PARENT,
                    window, screen->root,
                    0, 0, 150, 150, // Position and size.
                    10,            // Border width.
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual,
                    0, NULL);      // Masks, not used yet.  

  // Map the window on the screen.
  xcb_map_window(connection, window);

  // Get the delete window message atom.
  auto cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
  auto reply  = xcb_intern_atom_reply(connection, cookie, 0);
  cookie      = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
  auto wm_delete_window = xcb_intern_atom_reply(connection, cookie, 0);
  
  xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, (*reply).atom, 4, 32, 1,
                      &(*wm_delete_window).atom);  
  xcb_flush(connection);

  // Create a Vulkan instance.
  vk::instance instance;

  // Iterate over the available devices are choose the best one.
  const vk::physical_device *best_physical_device = nullptr;
  for (auto &device: instance.physical_devices()) {
    best_physical_device = &device;
  }
  
  if (nullptr == best_physical_device) {
    std::cerr << "No Vulkan compatible devices found. Exiting...\n";
    return -1;
  }

  // Create a surface.
  vk::surface surface{instance, connection, window};
 
  // We now need to query the device for supported queue familes and decide
  // what queues we need to construct.
  const vk::queue_family *family = nullptr;
  for (auto &queue_family: best_physical_device->queue_families()) {
//    if (queue_family.is_surface_supported(surface)/*1 || queue_family.is_presentation_supported(connection, window)*/) {
      family = &queue_family;
 //     break;
 //   }
  }
 
  if (nullptr == family) {
    std::cerr << "No queue family supporting presentation found. Exiting...\n";
  }

  // Now we can create a logical device.
  vk::device device{*best_physical_device};
  vk::queue queue = device.get_queue(family->index, 0);

  // Now choose our display surface format.
  auto format = choose_swapchain_format(*best_physical_device, surface);

  // Build a graphics pipeline.
  auto pipeline = make_graphics_pipeline(device);

   // Create a swap chain.
  vk::swapchain swapchain{device, surface, format};

  // Build a graphics pipeline.
//  auto pipeline = make_graphics_pipeline(device);

  // Create the command buffer pool.
  vk::command_pool command_pool{device, family->index};
  vk::command_buffer command_buffer = command_pool.allocate();
 // command_buffer.record([](vk::command_builder& builder) {
//    builder.pipeline_barrier(nullptr, 0, nullptr, 0, nullptr, 0);
//  });

  // Start the event loop.
  while (handle_events(connection, wm_delete_window->atom)) {
    // Grab an image and immediately present it.
    auto image = swapchain.acquire_next_image();
    command_buffer.record([&](vk::command_builder& builder) {
      builder.pipeline_barrier(nullptr, 0, nullptr, 0, nullptr, 0, image);
    });

    queue.submit(&command_buffer, 1);
    queue.present(image);
    queue.wait_idle();
  }

  queue.wait_idle();

  xcb_destroy_window(connection, window);
  xcb_disconnect(connection);

  return 0;
}
