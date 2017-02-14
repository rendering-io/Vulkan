#include <vk/vk.h>
#include <cassert>

using namespace vk;

class image_view::impl {
public:
  impl(image image);
  ~impl();

  image image_;
  VkImageView handle_;
};

image_view::impl::impl(image image)
: image_{std::move(image)}, handle_{VK_NULL_HANDLE} { }

image_view::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyImageView(image_.device(), handle_, nullptr);
  }
}

component_mapping::component_mapping() 
: r{swizzle::identity}, g{swizzle::identity}, b{swizzle::identity}, 
  a{swizzle::identity}
{}

image_view::image_view(image image, type view_type, texel_format format, 
                       component_mapping swizzle, subresource_range range)
: impl_{std::make_shared<impl>(std::move(image))} {
  VkImageViewCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.image = impl_->image_;
  info.viewType = static_cast<VkImageViewType>(view_type);
  info.format = static_cast<VkFormat>(format);
  info.components.r = static_cast<VkComponentSwizzle>(swizzle.r);
  info.components.g = static_cast<VkComponentSwizzle>(swizzle.g);
  info.components.b = static_cast<VkComponentSwizzle>(swizzle.b);
  info.components.a = static_cast<VkComponentSwizzle>(swizzle.a);
  info.subresourceRange.aspectMask = static_cast<VkImageAspectFlags>(range.aspect_mask);
  info.subresourceRange.baseMipLevel = range.base_mip_level;
  info.subresourceRange.levelCount = range.mip_count;
  info.subresourceRange.baseArrayLayer = range.base_array_layer;
  info.subresourceRange.layerCount = range.layer_count;

  auto result = vkCreateImageView(impl_->image_.device(), &info, nullptr, 
                                  &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create image view.");
}

image_view::operator VkImageView() {
  return impl_->handle_;
}
