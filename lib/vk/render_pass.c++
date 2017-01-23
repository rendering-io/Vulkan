#include <vk/vk.h>

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
: impl_{std::make_shared<impl>(std::move(device))} { }

render_pass::operator VkRenderPass() {
  return impl_->handle_;
}
