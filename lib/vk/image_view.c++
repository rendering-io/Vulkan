#include <vk/vk.h>

using namespace vk;

class image_view::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkImageView handle_;
};

image_view::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

image_view::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyImageView(device_, handle_, nullptr);
  }
}

image_view::image_view(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }
