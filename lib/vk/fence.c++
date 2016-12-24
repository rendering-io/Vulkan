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

fence::operator VkFence() {
  return impl_->handle_;
}

void fence::reset(fence *fences, uint32_t fence_count) {
  if (0 == fence_count)
    return;

  VkDevice device = fences[0].impl_->device_;
  std::vector<VkFence> fence_buf(fence_count);
  for (auto i = 0u; i < fence_count; ++i) {
    fence_buf[i] = fences[i];
    assert(device == fences[i].impl_->device_ && "All fences for reset must share a device.");
  }

  auto result = vkResetFences(device, fence_buf.size(), fence_buf.data());
  assert(VK_SUCCESS == result && "Failed to reset fences.");
}

void fence::reset() {
  fence::reset(this, 1);
}

wait_result fence::wait(fence *fences, uint32_t fence_count, bool wait_all,
                 uint64_t timeout) {
  if (0 == fence_count)
    return wait_result::SUCCESS;

  VkDevice device = fences[0].impl_->device_;
  std::vector<VkFence> fence_buf(fence_count);
  for (auto i = 0u; i < fence_count; ++i) {
    fence_buf[i] = fences[i];
    assert(device == fences[i].impl_->device_ && "All fences in a wait must share a device.");
  }

  auto result = vkWaitForFences(device, fence_buf.size(), fence_buf.data(), wait_all, timeout);
  switch (result) {
  case VK_SUCCESS:
    return wait_result::SUCCESS;
  case VK_TIMEOUT:
    return wait_result::TIMEOUT;
  default:
    assert(false && "Error waiting for fence.");
  }
}

wait_result fence::wait(uint64_t timeout) {
  return fence::wait(this, 1, false, timeout);
}

