#include <vk/vk.h>
#include <cassert>

using namespace vk;

class pipeline_layout::impl {
public:
  impl(device device);
  ~impl();
  
  device device_;
  VkPipelineLayout handle_;
};

pipeline_layout::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

pipeline_layout::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyPipelineLayout(device_, handle_, nullptr);
  }
}

pipeline_layout::pipeline_layout(device device, descriptor_set_layout* layouts,
                                 size_t layout_count)
: impl_{std::make_shared<impl>(device)}
{
  // Populate a vector of layout handles.
  std::vector<VkDescriptorSetLayout> layout_handles(layout_count);
  for (auto i = 0ul; i < layout_count; ++i)
    layout_handles[i] = layouts[i];

  VkPipelineLayoutCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.setLayoutCount = layout_count;
  info.pSetLayouts = layout_handles.data();
  info.pushConstantRangeCount = 0;
  info.pPushConstantRanges = nullptr;

  auto result = vkCreatePipelineLayout(impl_->device_, &info, nullptr,
                                       &impl_->handle_);
  assert(VK_SUCCESS == result &&
         "Failed to create pipeline layout.");
}
