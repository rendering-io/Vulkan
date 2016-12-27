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

surface::operator VkSurfaceKHR() {
  return impl_->handle_;
}
