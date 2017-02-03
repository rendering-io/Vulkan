#include <vk/vk.h>
#include <cassert>
#include "utility.h"

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

pipeline_shader::pipeline_shader(shader_stage stage, shader_module module,
                                 const char *entry_point)
: stage_{stage}, module_{module}, name_{entry_point} {}

const char *pipeline_shader::name() const {
  return name_;
}

shader_module pipeline_shader::module() const {
  return module_;
}

pipeline_shader::shader_stage pipeline_shader::stage() const {
  return stage_;
}

pipeline::pipeline(device device)
: impl_{std::make_shared<impl>(device)} { }

pipeline::operator VkPipeline() {
  return impl_->handle_;
}

compute_pipeline::compute_pipeline(device device, pipeline_layout layout,
                                   shader_module module, const char* entry_point)
: pipeline{device} {
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

rasterization_state::rasterization_state() {
  depth_clamp_enabled = false;
  rasterizer_discard_enabled = false;
  depth_bias_enabled = false;
  cull_mode = 2; // Cull back facing.
}

static void initialize_raster_state_create_info(VkPipelineRasterizationStateCreateInfo &info,
    const rasterization_state &raster_state) {
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.depthClampEnable = raster_state.depth_clamp_enabled;
  info.rasterizerDiscardEnable = raster_state.rasterizer_discard_enabled;
  info.polygonMode = VK_POLYGON_MODE_FILL;
  info.cullMode = raster_state.cull_mode;
  info.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
/*  info.sType;
  info.polygonMode;
  info.cullMode;
  info.frontFace;*/
  //info.cullMode = raster_state.cull_mode;
  info.depthBiasEnable = raster_state.depth_bias_enabled;
  info.depthBiasConstantFactor = 0.0f;
  info.depthBiasClamp = 0.0f;
  info.depthBiasSlopeFactor = 0.0f;
  info.lineWidth = 1.0f;
}

static void initialize_dynamic_state_create_info(VkPipelineDynamicStateCreateInfo &info
    ) {
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.dynamicStateCount = 0;
  info.pDynamicStates = nullptr;
}

graphics_pipeline::graphics_pipeline(device device, 
                                     const pipeline_shader *stages,
                                     uint32_t stage_count,
                                     const vertex_input_state &vertex_state,
                                     const input_assembly_state &assembly_state,
                                     const rasterization_state &raster_state,
                                     pipeline_layout layout,
                                     render_pass render_pass)
: pipeline{device} {
  VkPipelineCache cache = VK_NULL_HANDLE;

  auto pipeline_stages = transform(stages, stages + stage_count, 
      [](const pipeline_shader &stage) {
        VkPipelineShaderStageCreateInfo info;
        info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        info.pNext = nullptr;
        info.flags = 0;
        info.stage = static_cast<VkShaderStageFlagBits>(stage.stage());
        info.module = stage.module();
        info.pName = stage.name();
        info.pSpecializationInfo = nullptr;
        return info;
      });

  // Set up vertex input bindings.
  VkPipelineVertexInputStateCreateInfo vertex_info;
  vertex_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_info.pNext = nullptr;
  vertex_info.flags = 0;
  vertex_info.vertexBindingDescriptionCount = 0;
  vertex_info.pVertexBindingDescriptions = nullptr;
  vertex_info.vertexAttributeDescriptionCount = 0;
  vertex_info.pVertexAttributeDescriptions = nullptr;

  // Set up input assembly state.
  VkPipelineInputAssemblyStateCreateInfo assembly_info;
  assembly_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  assembly_info.pNext = nullptr;
  assembly_info.flags = 0;
  assembly_info.topology = static_cast<VkPrimitiveTopology>(assembly_state.topology);
  assembly_info.primitiveRestartEnable = assembly_state.primitive_restart_enabled;

  // Set up raster state.
  VkPipelineRasterizationStateCreateInfo raster_info;
  initialize_raster_state_create_info(raster_info, raster_state);

  VkPipelineDynamicStateCreateInfo dynamic_info;
  initialize_dynamic_state_create_info(dynamic_info);

  VkPipelineCreateFlags flags = 0;
  VkGraphicsPipelineCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = flags;
  info.stageCount = pipeline_stages.size();
  info.pStages    = pipeline_stages.data();
  info.pVertexInputState = &vertex_info;
  info.pInputAssemblyState = &assembly_info;
  info.pTessellationState = nullptr;
  info.pViewportState = nullptr;
  info.pRasterizationState = &raster_info;
  info.pMultisampleState = nullptr;
  info.pDepthStencilState = nullptr;
  info.pColorBlendState = nullptr;
  info.pDynamicState = dynamic_info.dynamicStateCount ? &dynamic_info : nullptr;
  info.layout = layout;
  info.renderPass = render_pass;
  info.subpass = 0;
  info.basePipelineHandle = VK_NULL_HANDLE;
  info.basePipelineIndex = -1;

  auto result = vkCreateGraphicsPipelines(impl_->device_, cache, 1, &info,
                                          nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result &&
         "Failed to create graphics pipeline.");
}

