#include <vk/vk.h>
#include <cassert>

using namespace vk;

class framebuffer::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkFramebuffer handle_;
};

framebuffer::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

framebuffer::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyFramebuffer(device_, handle_, nullptr);
  }
}

framebuffer::framebuffer(device device, render_pass pass,
                         image_view *attachments, size_t attachment_count,
                         uint32_t width, uint32_t height, uint32_t layers)
: impl_{std::make_shared<impl>(std::move(device))} {
  std::vector<VkImageView> image_views(attachment_count);
  for (auto i = 0ul; i < attachment_count; ++i)
    image_views[i] = attachments[i];

  VkFramebufferCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.renderPass = pass;
  info.attachmentCount = attachment_count;
  info.pAttachments = image_views.data();
  info.width = width;
  info.height = height;
  info.layers = layers;

  auto result = vkCreateFramebuffer(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create frame buffer.");
}
