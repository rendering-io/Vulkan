#include <vk/vk.h>
#include <cassert>

using namespace vk;

class descriptor_set::impl {
public:
  impl(device device, descriptor_pool pool, VkDescriptorSet handle);
  ~impl();

  device device_;
  descriptor_pool pool_;
  VkDescriptorSet handle_;
};

class descriptor_pool::impl {
public:
  impl(device device);
  ~impl();
  
  device device_;
  VkDescriptorPool handle_;
};

descriptor_set::impl::impl(device device, descriptor_pool pool, VkDescriptorSet handle)
: device_{device}, pool_{pool}, handle_{handle} {
}

descriptor_set::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkFreeDescriptorSets(device_, pool_, 1, &handle_);
  }
}

descriptor_pool::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

descriptor_pool::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyDescriptorPool(device_, handle_, nullptr);
  }
}

descriptor_set::descriptor_set(device device, descriptor_pool pool, VkDescriptorSet handle)
: impl_{std::make_shared<impl>(device, pool, handle)} 
{
}

descriptor_pool::descriptor_pool(device device, uint32_t max_sets)
: impl_{std::make_shared<impl>(device)}
{
  // TODO: This pool size is hard coded to match the storage buffers for
  // vector_add, which is clearly a hack.
  VkDescriptorPoolSize pool_size;
  pool_size.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
  pool_size.descriptorCount = 3;

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

descriptor_pool::operator VkDescriptorPool() {
  return impl_->handle_;
}

descriptor_set descriptor_pool::allocate(descriptor_set_layout layout) {
  VkDescriptorSetLayout layout_handle = layout;

  VkDescriptorSetAllocateInfo info;
  info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  info.pNext = 0;
  info.descriptorPool = impl_->handle_;
  info.descriptorSetCount = 1;
  info.pSetLayouts = &layout_handle;

  VkDescriptorSet handle;
  auto result = vkAllocateDescriptorSets(impl_->device_, &info, &handle);
  assert(VK_SUCCESS == result && "Failed to allocate descriptor set");

  return descriptor_set{impl_->device_, *this, handle};
}

