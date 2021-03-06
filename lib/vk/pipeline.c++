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

viewport_state::viewport_state(const viewport *viewports, 
                               const rect<2> *scissors, uint32_t count)
: viewports_{viewports}, scissors_{scissors}, count_{count}
{
}

static void initialize_viewport_state_create_info(VkPipelineViewportStateCreateInfo &info,
    const viewport_state &viewport_state) {
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.viewportCount = viewport_state.count_;
  info.pViewports = reinterpret_cast<const VkViewport*>(viewport_state.viewports_);
  info.scissorCount = viewport_state.count_;
  info.pScissors = reinterpret_cast<const VkRect2D*>(viewport_state.scissors_);
}

static void initialize_multisample_state_create_info(VkPipelineMultisampleStateCreateInfo &info
    ) {
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  info.sampleShadingEnable = VK_FALSE;
  info.minSampleShading = 0.0f;
  info.pSampleMask = nullptr;
  info.alphaToCoverageEnable = VK_FALSE;
  info.alphaToOneEnable = VK_FALSE;
}

static void initialize_colour_blend_state_create_info(
  VkPipelineColorBlendStateCreateInfo &info,
  VkPipelineColorBlendAttachmentState &attachment_info) {

  attachment_info.blendEnable = VK_FALSE;
  attachment_info.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
  attachment_info.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
  attachment_info.colorBlendOp = VK_BLEND_OP_ADD;
  attachment_info.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  attachment_info.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  attachment_info.alphaBlendOp = VK_BLEND_OP_ADD;
  attachment_info.colorWriteMask = VK_COLOR_COMPONENT_R_BIT |
                                   VK_COLOR_COMPONENT_G_BIT |
                                   VK_COLOR_COMPONENT_B_BIT |
                                   VK_COLOR_COMPONENT_A_BIT;

  info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.logicOpEnable = VK_FALSE;
  info.logicOp = VK_LOGIC_OP_COPY;
  info.attachmentCount = 1;
  info.pAttachments = &attachment_info;
  info.blendConstants[0] = 0.0f;  
  info.blendConstants[1] = 0.0f;  
  info.blendConstants[2] = 0.0f;  
  info.blendConstants[3] = 0.0f;  
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
                                     const viewport_state &viewport_state,
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

  // Set up viewport state.
  VkPipelineViewportStateCreateInfo viewport_info;
  initialize_viewport_state_create_info(viewport_info, viewport_state);
  
  // Set up raster state.
  VkPipelineRasterizationStateCreateInfo raster_info;
  initialize_raster_state_create_info(raster_info, raster_state);

  // Set up multisample state.
  VkPipelineMultisampleStateCreateInfo multisample_info;
  initialize_multisample_state_create_info(multisample_info);

  // Set up dynamic state.
  VkPipelineColorBlendStateCreateInfo colour_blend_info;
  VkPipelineColorBlendAttachmentState colour_blend_attachment_info;
  initialize_colour_blend_state_create_info(colour_blend_info, 
                                            colour_blend_attachment_info);

  // Set up dynamic state.
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
  info.pViewportState = &viewport_info;
  info.pRasterizationState = &raster_info;
  info.pMultisampleState = &multisample_info;
  info.pDepthStencilState = nullptr;
  info.pColorBlendState = &colour_blend_info;
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

