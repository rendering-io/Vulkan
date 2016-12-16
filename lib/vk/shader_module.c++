#include <vk/vk.h>
#include <cassert>

using namespace vk;

class shader_module::impl {
public:
  impl(device device);
  ~impl();

  VkShaderModule handle_;
  device device_;
};

shader_module::impl::impl(device device) : device_{device} {
  // Initialize the module handle to null.
  handle_ = VK_NULL_HANDLE;
}

shader_module::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyShaderModule(device_, handle_, nullptr);
  }
}

shader_module::shader_module(device device, const uint32_t* code,
                             size_t size_in_bytes)
: impl_{std::make_shared<impl>(std::move(device))} {
  assert(nullptr != code &&
         "Cannot create a shader module without an instruction stream.");
  assert(0 != size_in_bytes &&
         "Cannot create a shader module with a zero length instruction stream.");

  // The creation info for the module.
  VkShaderModuleCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.codeSize = size_in_bytes;
  info.pCode = code;

  auto result = vkCreateShaderModule(impl_->device_, &info, nullptr,
                                     &impl_->handle_);
  assert(VK_SUCCESS == result);
}

shader_module::operator VkShaderModule() {
  return impl_->handle_;
}
