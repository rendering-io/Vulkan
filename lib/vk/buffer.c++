#include <vk/vk.h>
#include <cassert>

using namespace vk;

class buffer::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkBuffer handle_;
  VkMemoryRequirements memory_requirements_;
};

buffer::impl::impl(device device)
: device_{device}, handle_{VK_NULL_HANDLE}
{
}

buffer::impl::~impl() {
  if (VK_NULL_HANDLE != handle_)
    vkDestroyBuffer(device_, handle_, nullptr);
}

buffer::buffer(device device, size_t size_in_bytes)
: impl_{std::make_shared<impl>(std::move(device))} {

  // Creation state for a buffer.
  VkBufferCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.size = size_in_bytes;
  info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  info.queueFamilyIndexCount = 0;
  info.pQueueFamilyIndices = nullptr;

  auto result = vkCreateBuffer(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Buffer creation failed.");

  vkGetBufferMemoryRequirements(impl_->device_, impl_->handle_,
                                &impl_->memory_requirements_);
}

buffer::operator VkBuffer() {
  return impl_->handle_;
}

void buffer::bind(device_memory memory, size_t offset, size_t /*size*/) {
  vkBindBufferMemory(impl_->device_, impl_->handle_, memory, offset);
}

