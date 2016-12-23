#include <vk/vk.h>

using namespace vk;

class buffer_view::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkBufferView handle_;
};

buffer_view::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

buffer_view::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyBufferView(device_, handle_, nullptr);
  }
}

buffer_view::buffer_view(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }
