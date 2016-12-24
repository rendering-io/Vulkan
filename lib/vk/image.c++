#include <vk/vk.h>
#include <cassert>

using namespace vk;

class image::impl {
public:
  impl(device device, VkImage handle, bool owns_handle);
  ~impl();

  device device_;
  VkImage handle_;
  bool owns_handle_;
};

image::impl::impl(device device, VkImage handle, bool owns_handle)
: device_{device}, handle_{handle}, owns_handle_{owns_handle}
{ }

image::impl::~impl() {
  if (owns_handle_ && VK_NULL_HANDLE != handle_) {
    vkDestroyImage(device_, handle_, nullptr);
  }
}

image::image(device device, VkImage handle, bool owns_handle)
: impl_{std::make_shared<impl>(device, handle, owns_handle)} {
}

void image::bind(device_memory memory, size_t offset, size_t /*size*/) {
  auto result = vkBindImageMemory(impl_->device_, impl_->handle_, memory, offset);
  assert(VK_SUCCESS == result && "Failed to bind image memory.");
}

