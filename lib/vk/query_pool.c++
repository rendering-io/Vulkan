#include <vk/vk.h>

using namespace vk;

class query_pool::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkQueryPool handle_;
};

query_pool::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

query_pool::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyQueryPool(device_, handle_, nullptr);
  }
}

query_pool::query_pool(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }
