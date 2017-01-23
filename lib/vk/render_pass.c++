#include <vk/vk.h>
#include <cassert>

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

render_pass::render_pass(device device)
: impl_{std::make_shared<impl>(std::move(device))} {
  VkRenderPassCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.attachmentCount = 0;
  info.pAttachments = nullptr;
  info.subpassCount = 0;
  info.pSubpasses = nullptr;
  info.dependencyCount = 0;
  info.pDependencies = nullptr;

  auto result = vkCreateRenderPass(impl_->device_, &info, nullptr, 
                                   &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create render pass.");
}

render_pass::operator VkRenderPass() {
  return impl_->handle_;
}
