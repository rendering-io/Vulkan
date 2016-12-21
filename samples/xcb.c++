#define VK_USE_PLATFORM_XCB_KHR
#include <vk/vk.h>
#include <xcb/xcb.h>
#include <memory>

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

  // Create a surface.
  vk::surface surface{instance, connection, window};
  
  // Start the event loop.
  while (handle_events(connection, wm_delete_window->atom)) {

  }

  xcb_destroy_window(connection, window);
  xcb_disconnect(connection);

  return 0;
}
