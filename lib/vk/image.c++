#include <vk/vk.h>
#include <cassert>

using namespace vk;

class image::impl {
public:
  impl(device device, VkImage handle, bool owns_handle);
  ~impl();

  device device_;
  VkImage handle_;
  VkMemoryRequirements memory_requirements_;
  bool owns_handle_;
};

image::impl::impl(device device, VkImage handle, bool owns_handle)
: device_{device}, handle_{handle}, owns_handle_{owns_handle}
{ }

image::impl::~impl() {
  if (owns_handle_ && VK_NULL_HANDLE != handle_) {
    vkDestroyImage(device_, handle_, nullptr);
  }
}

image::image(device device, VkImage handle, bool owns_handle)
: impl_{std::make_shared<impl>(device, handle, owns_handle)} {
  vkGetImageMemoryRequirements(impl_->device_, impl_->handle_, &impl_->memory_requirements_);
}

image::image(device device, texel_format format, extent<3> extent, uint32_t mip_levels, uint32_t array_layers)
: impl_{std::make_shared<impl>(device, static_cast<VkImage>(VK_NULL_HANDLE),
                               true)} {
  VkImageCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.imageType = VK_IMAGE_TYPE_2D;
  info.format = static_cast<VkFormat>(format);
  info.extent.width = extent.width;
  info.extent.height = extent.height;
  info.extent.depth = extent.depth;
  info.mipLevels = mip_levels;
  info.arrayLayers = array_layers;
  info.samples = VK_SAMPLE_COUNT_1_BIT;
  info.tiling = VK_IMAGE_TILING_OPTIMAL;
  info.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
  info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  info.queueFamilyIndexCount = 0;
  info.pQueueFamilyIndices = nullptr;
  info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  auto result = vkCreateImage(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create image.");
}

image::operator VkImage() {
  return impl_->handle_;
}

void image::bind(device_memory memory, size_t offset, size_t /*size*/) {
  auto result = vkBindImageMemory(impl_->device_, impl_->handle_, memory, offset);
  assert(VK_SUCCESS == result && "Failed to bind image memory.");
}

size_t image::minimum_allocation_alignment() const {
  return impl_->memory_requirements_.alignment;
}

size_t image::minimum_allocation_size() const {
  return impl_->memory_requirements_.size;
}

