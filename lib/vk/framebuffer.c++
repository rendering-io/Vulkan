#include <vk/vk.h>

using namespace vk;

class framebuffer::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkFramebuffer handle_;
};

framebuffer::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

framebuffer::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyFramebuffer(device_, handle_, nullptr);
  }
}

framebuffer::framebuffer(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }
