#include <vk/vk.h>
#include <cassert>

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

fence::fence(device device, bool signaled)
: impl_{std::make_shared<impl>(std::move(device))} {
  VkFenceCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

  auto result = vkCreateFence(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create fence.");
}

