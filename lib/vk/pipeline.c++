#include <vk/vk.h>
#include <cassert>

using namespace vk;

class pipeline::impl {
public:
  impl(device device);
  ~impl();
  
  device device_;
  VkPipeline handle_;
};

pipeline::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

pipeline::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyPipeline(device_, handle_, nullptr);
  }
}

pipeline::pipeline(device device, pipeline_layout layout,
                   shader_module module, const char* entry_point)
: impl_{std::make_shared<impl>(device)}
{
  VkPipelineCache cache = VK_NULL_HANDLE;

  VkPipelineShaderStageCreateInfo stage;
  stage.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stage.pNext  = nullptr;
  stage.flags  = 0;
  stage.stage  = VK_SHADER_STAGE_COMPUTE_BIT;
  stage.module = module;
  stage.pName   = entry_point;
  stage.pSpecializationInfo = nullptr;

  VkPipelineCreateFlags flags = 0;
  VkComputePipelineCreateInfo info;
  info.sType  = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
  info.pNext  = nullptr;
  info.flags  = flags;
  info.stage  = stage;
  info.layout = layout;
  info.basePipelineHandle = VK_NULL_HANDLE;
  info.basePipelineIndex = -1;

  auto result = vkCreateComputePipelines(impl_->device_, cache, 1, &info,
                                         nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result &&
         "Failed to create compute pipeline.");
}

pipeline::operator VkPipeline() {
  return impl_->handle_;
}

