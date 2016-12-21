#include <vk/vk.h>
#include <cassert>

using namespace vk;

class swapchain::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkSwapchainKHR handle_;
};

swapchain::impl::impl(device device)
: device_{device}, handle_{VK_NULL_HANDLE}
{
}

swapchain::impl::~impl() {
  if (VK_NULL_HANDLE != handle_)
    vkDestroySwapchainKHR(device_, handle_, nullptr);
}

swapchain::swapchain(device device)
: impl_{std::make_shared<impl>(std::move(device))} {

  // Creation state for a swapchain.
  VkSwapchainCreateInfoKHR info;
/*
typedef struct VkSwapchainCreateInfoKHR {
    VkStructureType                  sType;
    const void*                      pNext;
    VkSwapchainCreateFlagsKHR        flags;
    VkSurfaceKHR                     surface;
    uint32_t                         minImageCount;
    VkFormat                         imageFormat;
    VkColorSpaceKHR                  imageColorSpace;
    VkExtent2D                       imageExtent;
    uint32_t                         imageArrayLayers;
    VkImageUsageFlags                imageUsage;
    VkSharingMode                    imageSharingMode;
    uint32_t                         queueFamilyIndexCount;
    const uint32_t*                  pQueueFamilyIndices;
    VkSurfaceTransformFlagBitsKHR    preTransform;
    VkCompositeAlphaFlagBitsKHR      compositeAlpha;
    VkPresentModeKHR                 presentMode;
    VkBool32                         clipped;
    VkSwapchainKHR                   oldSwapchain;
} VkSwapchainCreateInfoKHR;
*/
  auto result = vkCreateSwapchainKHR(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Swap chain creation failed.");
}

swapchain::operator VkSwapchainKHR() {
  return impl_->handle_;
}
