#include <vk/vk.h>
#include <cassert>

using namespace vk;

class swapchain::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkSwapchainKHR handle_;
  std::vector<swapchain_image> images_;
};

swapchain::impl::impl(device device)
: device_{device}, handle_{VK_NULL_HANDLE}
{
}

swapchain::impl::~impl() {
  if (VK_NULL_HANDLE != handle_)
    vkDestroySwapchainKHR(device_, handle_, nullptr);
}

swapchain_image::swapchain_image(device device, swapchain swapchain, VkImage handle, uint32_t index)
: image{device, handle, false}, swapchain_{swapchain}, index_{index} { }

swapchain::swapchain(device device, surface surface, surface_format format)
: impl_{std::make_shared<impl>(std::move(device))} {

  auto &physical_device = impl_->device_.physical_device();

  // Query the surface capabilities.
  VkSurfaceCapabilitiesKHR caps;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &caps);

  // Query available present modes.
  VkPresentModeKHR best_present_mode = VK_PRESENT_MODE_IMMEDIATE_KHR;
  uint32_t present_mode_count = 0;
  auto result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                                          surface,
                                                          &present_mode_count, nullptr);
  if (VK_SUCCESS == result) {
    std::vector<VkPresentModeKHR> present_modes(present_mode_count);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device,
                                                       surface,
                                                       &present_mode_count,
                                                       present_modes.data());
    if (VK_SUCCESS == result) {
      for (auto mode: present_modes) {
        if (mode == VK_PRESENT_MODE_FIFO_KHR)
          best_present_mode = mode;
      }
    }
  }

  // Creation state for a swapchain.
  VkSwapchainCreateInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  info.pNext = nullptr;
  info.flags = 0;
  info.surface = surface;
  info.minImageCount = 3;
  info.imageFormat = static_cast<VkFormat>(format.format);
  info.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
  info.imageExtent = caps.currentExtent;
  info.imageArrayLayers = 1;
  info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  info.queueFamilyIndexCount = 0;
  info.pQueueFamilyIndices = nullptr;
  info.preTransform = caps.currentTransform;
  info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  info.presentMode = best_present_mode;
  info.clipped = VK_TRUE;
  info.oldSwapchain = VK_NULL_HANDLE;
  
  result = vkCreateSwapchainKHR(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Swap chain creation failed.");

  uint32_t image_count = 0;
  std::vector<VkImage> swapchain_images;
  result = vkGetSwapchainImagesKHR(impl_->device_, impl_->handle_, &image_count, nullptr);
  if (VK_SUCCESS == result) {
    swapchain_images.resize(image_count);
    result = vkGetSwapchainImagesKHR(impl_->device_, impl_->handle_, &image_count,
                                     swapchain_images.data());
  }

  for (auto i = 0ul; i < swapchain_images.size(); ++i) { 
    impl_->images_.push_back(swapchain_image(impl_->device_, *this, swapchain_images[i], i));
  }

  // We have all the images, but they still need binding to memory.
  // First we need to calculate the memory requirements.
  std::vector<std::pair<size_t, size_t>> subregions;
  size_t total_size = 0;
  for (auto &image: impl_->images_) {
    // Satisfy the alignment requirements.
    auto alignment = image.minimum_allocation_alignment();
    if (0 != (total_size % alignment))
      total_size = (total_size + alignment - 1) / alignment * alignment;
    
    auto size = image.minimum_allocation_size();
    subregions.emplace_back(total_size, size);
    total_size += size;
  }

  // Allocate a block of device memory.
  const physical_device::memory_type *best_memory_pool = nullptr;
  for (auto &pool: impl_->device_.physical_device().memory_types()) {
    if (nullptr == best_memory_pool)
      best_memory_pool = &pool;

    if (pool.is_device_local())
      best_memory_pool = &pool;
  }

  device_memory memory{impl_->device_, *best_memory_pool, total_size};
  for (auto i = 0ul; i < impl_->images_.size(); ++i) {
    impl_->images_[i].bind(memory, subregions[i].first, subregions[i].second);
  }
}

swapchain::operator VkSwapchainKHR() {
  return impl_->handle_;
}

swapchain_image swapchain::acquire_next_image() {
  // Declare a fence so we can wait for the next available image. Initially unsignaled.
  fence fence{impl_->device_, false};
  
  // Acquire the index for the next image.
  uint32_t index = 0;
  auto result = vkAcquireNextImageKHR(impl_->device_, impl_->handle_, UINT64_MAX,
                                      VK_NULL_HANDLE, fence, &index);
  assert(VK_SUCCESS == result && "Failed to acquire swapchain image.");

  // Wait on the semaphore until we can actually use the image.
  auto status = fence.wait(UINT64_MAX);
  assert(wait_result::SUCCESS == status);
  return impl_->images_[index];
}

