#include <vk/vk.h>
#include <cassert>
#include "utility.h"

using namespace vk;

class render_pass::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkRenderPass handle_;
};

render_pass::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

render_pass::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyRenderPass(device_, handle_, nullptr);
  }
}

attachment_reference::attachment_reference(uint32_t attachment_index,
                                           image_layout layout) {
  ref_.attachment = attachment_index;
  ref_.layout = static_cast<VkImageLayout>(layout);
}

attachment_description::attachment_description(texel_format format,
                                               load_operation load_op,
                                               store_operation store_op,
                                               load_operation stencil_load_op,
                                               store_operation stencil_store_op,
                                               image_layout initial_layout,
                                               image_layout final_layout) {
  desc_.flags = 0;
  desc_.format = static_cast<VkFormat>(format);
  desc_.samples = VK_SAMPLE_COUNT_1_BIT;
  desc_.loadOp = static_cast<VkAttachmentLoadOp>(load_op);
  desc_.storeOp = static_cast<VkAttachmentStoreOp>(store_op);
  desc_.stencilLoadOp = static_cast<VkAttachmentLoadOp>(stencil_load_op);
  desc_.stencilStoreOp = static_cast<VkAttachmentStoreOp>(stencil_store_op);
  desc_.initialLayout = static_cast<VkImageLayout>(initial_layout);
  desc_.finalLayout = static_cast<VkImageLayout>(final_layout);
}

subpass_description::subpass_description(const attachment_reference *input_attachments,
                                         uint32_t input_attachment_count,
                                         const attachment_reference *colour_attachments,
                                         uint32_t colour_attachment_count,
                                         const attachment_reference *resolve_attachments,
                                         const attachment_reference *depth_stencil_attachment,
                                         const uint32_t *preserve_attachments,
                                         uint32_t preserve_attachment_count) {
  desc_.flags = 0;
  desc_.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  desc_.pInputAttachments = reinterpret_cast<const VkAttachmentReference*>(input_attachments);
  desc_.inputAttachmentCount = input_attachment_count;
  desc_.pColorAttachments = reinterpret_cast<const VkAttachmentReference*>(colour_attachments);
  desc_.colorAttachmentCount = colour_attachment_count;
  desc_.pResolveAttachments = reinterpret_cast<const VkAttachmentReference*>(resolve_attachments);
  desc_.pDepthStencilAttachment = reinterpret_cast<const VkAttachmentReference*>(depth_stencil_attachment);
  desc_.pPreserveAttachments = preserve_attachments;
  desc_.preserveAttachmentCount = preserve_attachment_count;
}

render_pass::render_pass(device device,
                         const attachment_description *attachments,
                         uint32_t attachment_count,
                         const subpass_description *subpasses,
                         uint32_t subpass_count,
                         const subpass_dependency *dependencies,
                         uint32_t dependency_count)
: impl_{std::make_shared<impl>(std::move(device))} {
  // Convert subpasses to VkSubpass
  VkRenderPassCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.attachmentCount = attachment_count;
  info.pAttachments = reinterpret_cast<const VkAttachmentDescription*>(attachments);
  info.subpassCount = subpass_count; 
  info.pSubpasses = reinterpret_cast<const VkSubpassDescription*>(subpasses); 
  info.dependencyCount = dependency_count;
  info.pDependencies = reinterpret_cast<const VkSubpassDependency*>(dependencies);

  auto result = vkCreateRenderPass(impl_->device_, &info, nullptr, 
                                   &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create render pass.");
}

render_pass::operator VkRenderPass() {
  return impl_->handle_;
}
