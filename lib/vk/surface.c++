#include <vk/vk.h>
#include <cassert>

using namespace vk;

class surface::impl {
public:
  impl(instance instance);
  ~impl();

  instance instance_;
  VkSurfaceKHR handle_;
};

surface::impl::impl(instance instance)
: instance_{instance}, handle_{VK_NULL_HANDLE}
{
}

surface::impl::~impl() {
  if (VK_NULL_HANDLE != handle_)
    vkDestroySurfaceKHR(instance_, handle_, nullptr);
}

#ifdef VK_USE_PLATFORM_XLIB_KHR
surface::surface(instance instance, Display *display, Window window)
: impl_{std::make_shared<impl>(std::move(instance))} {

  // Creation state for a surface.
  VkXlibSurfaceCreateInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
  info.pNext = nullptr;
  info.flags = 0;
  info.dpy = dpy;
  info.window = window;

  auto result = vkCreateXlibSurfaceKHR(impl_->instance_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Surface creation failed.");
}
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
surface::surface(instance instance,
                 xcb_connection_t *connection, xcb_window_t window)
: impl_{std::make_shared<impl>(std::move(instance))} {

  // Creation state for a surface.
  VkXcbSurfaceCreateInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
  info.pNext = nullptr;
  info.flags = 0;
  info.connection = connection;
  info.window = window;

  auto result = vkCreateXcbSurfaceKHR(impl_->instance_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Surface creation failed.");
}
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
surface::surface(instance instance, MirConnection *connection, MirSurface *surface)
: impl_{std::make_shared<impl>(std::move(instance))} {

  VkMirSurfaceCreateInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_MIR_SURFACE_CREATE_INFO_KHR;
  info.pNext = nullptr;
  info.flags = 0;
  info.connection = connection;
  info.surface = surface;

  auto result = vkCreateMirSurfaceKHR(instance, &info, nullptr, impl_->handle_);
  assert(VK_SUCCESS == result && "Surface creation failed.");
}
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
surface::surface(instance instance, wl_display *display, wl_surface *surface)
: impl_{std::make_shared<impl>(std::move(instance))} {

  VkWaylandSurfaceCreateInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR;
  info.pNext = nullptr;
  info.flags = 0;
  info.display = display;
  info.surface = surface;

  auto result = vkCreateWaylandSurfaceKHR(instance, &info, nullptr, impl_->handle_);
  assert(VK_SUCCESS == result && "Surface creation failed.");
}
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
surface::surface(instance instance, ANativeWindow *window)
: impl_{std::make_shared<impl>(std::move(instance))} {

  VkAndroidSurfaceCreateInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
  info.pNext = nullptr;
  info.flags = 0;
  info.window = window;

  auto result = vkCreateAndroidSurfaceKHR(instance, &info, nullptr, impl_->handle_);
  assert(VK_SUCCESS == result && "Surface creation failed.");
}
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
surface::surface(instance instance, HINSTANCE instance, HWND window)
: impl_{std::make_shared<impl>(std::move(instance))} {

  VkWin32SurfaceCreateInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  info.pNext = nullptr;
  info.flags = 0;
  info.hinstance = instance;
  info.hwnd = window;

  auto result = vkCreateWin32SurfaceKHR(instance, &info, nullptr, impl_->handle_);
  assert(VK_SUCCESS == result && "Surface creation failed.");
}
#endif

surface::operator VkSurfaceKHR() {
  return impl_->handle_;
}
