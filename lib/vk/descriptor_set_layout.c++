#include <vk/vk.h>
#include <cassert>

using namespace vk;

class descriptor_set_layout::impl {
public:
  impl(device device);
  ~impl();
  
  device device_;
  VkDescriptorSetLayout handle_;
};

descriptor_set_layout::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

descriptor_set_layout::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyDescriptorSetLayout(device_, handle_, nullptr);
  }
}

descriptor_set_layout::descriptor_set_layout(device device, void*, size_t binding_count)
: impl_{std::make_shared<impl>(device)}
{
  // Populate a vector of layout handles.
  std::vector<VkDescriptorSetLayoutBinding> bindings(binding_count);
  for (auto i = 0ul; i < binding_count; ++i) {
    assert(false && "Creation of bindings not yet implemented.");
    /*
    bindings[i].binding = ;
    bindings[i].descriptorType = ;
    bindings[i].descriptorCount = ;
    bindings[i].stageFlags = ;
    bindings[i].pImmutableSamplers = ;
    */
  }

  VkDescriptorSetLayoutCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.bindingCount = binding_count;
  info.pBindings = bindings.data();

  auto result = vkCreateDescriptorSetLayout(impl_->device_, &info, nullptr,
                                       &impl_->handle_);
  assert(VK_SUCCESS == result &&
         "Failed to create descriptor set layout.");
}

descriptor_set_layout::operator VkDescriptorSetLayout() {
  return impl_->handle_;
}
