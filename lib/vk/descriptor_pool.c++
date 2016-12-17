#include <vk/vk.h>
#include <cassert>

using namespace vk;

class descriptor_pool::impl {
public:
  impl(device device);
  ~impl();
  
  device device_;
  VkDescriptorPool handle_;
};

descriptor_pool::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

descriptor_pool::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyDescriptorPool(device_, handle_, nullptr);
  }
}

descriptor_pool::descriptor_pool(device device, uint32_t max_sets)
: impl_{std::make_shared<impl>(device)}
{
  VkDescriptorPoolSize pool_size;
  pool_size.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  pool_size.descriptorCount = 1;

  VkDescriptorPoolCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.maxSets = max_sets;
  info.poolSizeCount = 1;
  info.pPoolSizes = &pool_size;

  auto result = vkCreateDescriptorPool(impl_->device_, &info, nullptr,
                                       &impl_->handle_);
  assert(VK_SUCCESS == result &&
         "Failed to create descriptor pool.");
}
