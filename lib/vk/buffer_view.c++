#include <vk/vk.h>
#include <cassert>

using namespace vk;

class buffer_view::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkBufferView handle_;
};

buffer_view::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

buffer_view::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyBufferView(device_, handle_, nullptr);
  }
}

buffer_view::buffer_view(device device, buffer buffer, texel_format format,
                         size_t offset, size_t range)
: impl_{std::make_shared<impl>(std::move(device))}
{
  VkBufferViewCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.buffer = buffer;
  info.format = static_cast<VkFormat>(format);
  info.offset = offset;
  info.range = range;

  auto result = vkCreateBufferView(impl_->device_, &info, nullptr, 
                                   &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create buffer view.");
}

