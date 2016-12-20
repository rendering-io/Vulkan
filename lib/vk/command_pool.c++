#include <vk/vk.h>
#include <cassert>

using namespace vk;

class command_pool::impl {
public:
  impl(device device);
  ~impl();
  
  device device_;
  VkCommandPool handle_;
};

command_pool::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

command_pool::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyCommandPool(device_, handle_, nullptr);
  }
}

command_pool::command_pool(device device, uint32_t queue_family)
: impl_{std::make_shared<impl>(device)}
{
  VkCommandPoolCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | 
               VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  info.queueFamilyIndex = queue_family;

  auto result = vkCreateCommandPool(impl_->device_, &info, nullptr,
                                    &impl_->handle_);
  assert(VK_SUCCESS == result &&
         "Failed to create command pool.");
}

command_pool::operator VkCommandPool() {
  return impl_->handle_;
}

command_buffer command_pool::allocate() {

  VkCommandBufferAllocateInfo info;
  info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  info.pNext = nullptr;
  info.commandPool = *this;
  info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  info.commandBufferCount = 1;

  VkCommandBuffer handle = VK_NULL_HANDLE;
  auto result = vkAllocateCommandBuffers(impl_->device_, &info, &handle);
  assert(VK_SUCCESS == result && "Failed to allocate command buffer.");

  return command_buffer{impl_->device_, *this, handle};
}

