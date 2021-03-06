#include <vk/vk.h>
#include <cassert>
#include <iostream>
#include "io.h"

int main(int argc, char **argv) {
  // The number of elements to add.
  const size_t ELEMENT_COUNT = 1024;

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

  // We now need to query the device for supported queue familes and decide
  // what queues we need to construct.
  const vk::queue_family *family = nullptr;
  for (auto &queue_family: best_physical_device->queue_families()) {
    std::cout << "Queue family " << queue_family.index << "\n";
    family = &queue_family;
    break;
  }

  // Now we can create a logical device.
  vk::device device{*best_physical_device};
  vk::queue queue = device.get_queue(family->index, 0);

  // Now we want to find a host-coherent memory region.
  const vk::physical_device::memory_type* best_memory_type = nullptr;
  for (auto &memory_type: best_physical_device->memory_types()) {
    if (memory_type.is_host_coherent())
      best_memory_type = &memory_type;
  }

  if (nullptr == best_memory_type) {
    std::cerr << "Unable to find a host-coherent memory type. Exiting...";
    return -1;
  }

  // Create three buffers.
  auto size_in_bytes = ELEMENT_COUNT * sizeof(uint32_t);
  vk::buffer a{device, size_in_bytes};
  vk::buffer b{device, size_in_bytes};
  vk::buffer c{device, size_in_bytes};

  // We should be calling get_memory_requirements for each buffer here.
  vk::device_memory storage{device, *best_memory_type, 3 * size_in_bytes};
  a.bind(storage, 0, size_in_bytes);
  b.bind(storage, size_in_bytes, size_in_bytes);
  c.bind(storage, 2 * size_in_bytes, size_in_bytes);

  // Map the range of buffers a, b.
  void *ptr = nullptr;
  if (map_memory(storage, 0, size_in_bytes, &ptr)) {
    uint32_t* as_uint32 = reinterpret_cast<uint32_t*>(ptr);
    for (auto i = 0ul; i < ELEMENT_COUNT; ++i)
      as_uint32[i] = i;

   unmap_memory(storage);
  }

  if (map_memory(storage, size_in_bytes, size_in_bytes, &ptr)) {
    uint32_t* as_uint32 = reinterpret_cast<uint32_t*>(ptr);
    for (auto i = 0ul; i < ELEMENT_COUNT; ++i)
      as_uint32[i] = i;

   unmap_memory(storage);
  }

  // Load the shader module.
  auto spirv = load_shader("vector_add.spv");
  vk::shader_module shader{device, spirv.data(), spirv.size() * sizeof(uint32_t)};

  // Define some layout bindings.
  std::vector<vk::descriptor_set_layout_binding> layout_bindings;
  layout_bindings.emplace_back(0);
  layout_bindings.emplace_back(1);
  layout_bindings.emplace_back(2);

  vk::descriptor_set_layout layout{device, layout_bindings.data(), layout_bindings.size()};
  vk::pipeline_layout pipeline_layout{device, &layout, 1}; 

  // Now construct a pipeline.
  vk::compute_pipeline pipeline{device, pipeline_layout, shader, "main"};

  // While we have defined the layout of our descriptor set, we haven't 
  // actually bound the buffers into a concrete descriptor. Now we create
  // the real descriptor.
  vk::descriptor_pool descriptor_pool{device, 1};
  vk::descriptor_set descriptor = descriptor_pool.allocate(layout);

  std::vector<vk::descriptor_binding> bindings;
  bindings.emplace_back(0, a);
  bindings.emplace_back(1, b);
  bindings.emplace_back(2, c);
  descriptor.update(bindings.data(), bindings.size());

  // Once all our bindings are set up we can build a command buffer.
  vk::command_pool command_pool{device, family->index};
  vk::command_buffer cmd = command_pool.allocate();
  cmd.begin();
  cmd.bind_pipeline(pipeline);
  cmd.bind_descriptor_sets(pipeline_layout, &descriptor, 1);
  cmd.dispatch(ELEMENT_COUNT);
  cmd.end();

  // Submit and wait for the queue to be idle.
  queue.submit(&cmd, 1);
  queue.wait_idle();
  std::cout << "Done waiting for idle.\n";

  // Validate results.
  uint32_t correct = 0;
  if (map_memory(storage, 2* size_in_bytes, size_in_bytes, &ptr)) {
    uint32_t* as_uint32 = reinterpret_cast<uint32_t*>(ptr);
    for (auto i = 0ul; i < ELEMENT_COUNT; ++i) {
      if (as_uint32[i] == 2 * i)
        ++correct;
    }
    unmap_memory(storage);
  }

  std::cout << correct << "/" << ELEMENT_COUNT << " results correct.\n";
  return 0;
}

