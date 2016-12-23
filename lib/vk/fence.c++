#include <vk/vk.h>

using namespace vk;

class fence::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkFence handle_;
};

fence::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

fence::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyFence(device_, handle_, nullptr);
  }
}

fence::fence(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }
