#include <vk/vk.h>

using namespace vk;

class semaphore::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkSemaphore handle_;
};

semaphore::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

semaphore::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroySemaphore(device_, handle_, nullptr);
  }
}

semaphore::semaphore(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }
