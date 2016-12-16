#include <vk/vk.h>

using namespace vk;

class buffer::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkBuffer handle_;
};

buffer::impl::impl(device device)
: device_{device}, handle_{VK_NULL_HANDLE}
{
}

buffer::impl::~impl() {
  if (VK_NULL_HANDLE != handle_)
    vkDestroyBuffer(device_, handle_, nullptr);
}

buffer::buffer(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }

