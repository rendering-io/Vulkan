#include <vk/vk.h>
#include <cassert>

using namespace vk;

class device_memory::impl {
public:
  impl(device);
  ~impl();

  device device_;
  VkDeviceMemory handle_;
};

device_memory::impl::impl(device device)
: device_{device}, handle_{VK_NULL_HANDLE} { }

device_memory::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkFreeMemory(device_, handle_, nullptr);
  }
}

device_memory::device_memory(device device, const physical_device::memory_type &memory_type, size_t size)
: impl_{std::make_shared<impl>(device)}
{
  VkMemoryAllocateInfo info;
  info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  info.pNext = nullptr;
  info.allocationSize = size;
  info.memoryTypeIndex = memory_type.index;

  auto result = vkAllocateMemory(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result &&
         "Failed to allocate device memory.");
}

