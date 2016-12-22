#include <vk/vk.h>
#include <cassert>

using namespace vk;

class swapchain::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkSwapchainKHR handle_;
  std::vector<VkImage> images_;
};

swapchain::impl::impl(device device)
: device_{device}, handle_{VK_NULL_HANDLE}
{
}

swapchain::impl::~impl() {
  if (VK_NULL_HANDLE != handle_)
    vkDestroySwapchainKHR(device_, handle_, nullptr);
}

swapchain_image::swapchain_image(swapchain swapchain, uint32_t index)
: swapchain_{swapchain}, index_{index} { }

swapchain::swapchain(device device, surface surface)
: impl_{std::make_shared<impl>(std::move(device))} {

  // Query the surface capabilities.
  VkSurfaceCapabilitiesKHR caps;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(impl_->device_.physical_device(), surface, &caps);

  // Creation state for a swapchain.
  VkSwapchainCreateInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  info.pNext = nullptr;
  info.flags = 0;
  info.surface = surface;
  info.minImageCount = 3;
  info.imageFormat = VK_FORMAT_R8G8B8_UNORM;
  info.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  info.imageExtent = caps.currentExtent;
  info.imageArrayLayers = 1;
  info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  info.queueFamilyIndexCount = 0;
  info.pQueueFamilyIndices = nullptr;
  info.preTransform = caps.currentTransform;
  info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  info.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  info.clipped = VK_TRUE;
  info.oldSwapchain = VK_NULL_HANDLE;
  
  auto result = vkCreateSwapchainKHR(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Swap chain creation failed.");

  uint32_t count = 0;
  result = vkGetSwapchainImagesKHR(impl_->device_, impl_->handle_, &count, nullptr);
  if (VK_SUCCESS == result) {
    impl_->images_.resize(count);
    result = vkGetSwapchainImagesKHR(impl_->device_, impl_->handle_, &count, impl_->images_.data());
  }
}

swapchain::operator VkSwapchainKHR() {
  return impl_->handle_;
}

swapchain_image swapchain::acquire_next_image() {
  uint32_t index = 0;
  auto result = vkAcquireNextImageKHR(impl_->device_, impl_->handle_, UINT64_MAX,
                                      VK_NULL_HANDLE, VK_NULL_HANDLE, &index);
  assert(VK_SUCCESS == result && "Failed to acquire swapchain image.");
  return swapchain_image{*this, index};
}

