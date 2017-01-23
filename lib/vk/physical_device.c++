#include <vk/vk.h>
#include <cassert>

using namespace vk;

queue_family::queue_family(physical_device &physical_device, uint32_t index,
                           uint32_t count, VkQueueFlags flags)
: index{index}, count{count}, flags_{flags}, physical_device_{physical_device}
{ 
}

bool queue_family::is_surface_supported(surface surface) const {
  VkBool32 supported = false;
  auto result = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device_, index,
                                                     surface, &supported);
  assert(VK_SUCCESS == result && "Failure querying queue family support for surface.");

  return supported;
}

#ifdef VK_USE_PLATFORM_XLIB_KHR
bool queue_family::is_presentation_supported(Display *display,
                                             VisualID visual) const {
  return vkGetPhysicalDeviceXlibPresentationSupportKHR(physical_device_,
                                                      index, display,
                                                      visual);
}
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
bool queue_family::is_presentation_supported(xcb_connection_t *connection,
                                             xcb_visualid_t visual) const {
  return vkGetPhysicalDeviceXcbPresentationSupportKHR(physical_device_,
                                                      index, connection,
                                                      visual);
}
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
bool queue_family::is_presentation_supported(wl_display *display) const {
  return vkGetPhysicalDeviceWaylandPresentationSupportKHR(physical_device_,
                                                          index, display);
}
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
bool queue_family::is_presentation_supported(iMirConnection *connection) const {
  return vkGetPhysicalDeviceMirPresentationSupportKHR(physical_device_,
                                                      index, connection);
}
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
bool queue_family::is_presentation_supported() const {
  return true;
}
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
bool queue_family::is_presentation_supported() const {
  return vkGetPhysicalDeviceWin32PresentationSupportKHR(physical_device_, index);
}
#endif

physical_device::memory_type::memory_type(uint32_t index,
                                          const VkMemoryType& type,
                                          const VkMemoryHeap& heap)
: index{index}, type_{type}, heap_{heap} {

}

bool physical_device::memory_type::is_device_local() const {
  return type_.propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
}

bool physical_device::memory_type::is_host_visible() const {
  return type_.propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
}

bool physical_device::memory_type::is_host_coherent() const {
  return type_.propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
}

bool physical_device::memory_type::is_host_cached() const {
  return type_.propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT;
}

bool physical_device::memory_type::is_lazily_allocated() const {
  return type_.propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT;
}

physical_device::physical_device(VkPhysicalDevice handle)
: handle_{handle} {
  if (0 == handle)
    return;

  // Get the device properties and features.
  vkGetPhysicalDeviceProperties(handle_, &properties_);
  vkGetPhysicalDeviceFeatures(handle_, &features_);

  // Get the queue families for the device.
  uint32_t count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(handle_, &count, nullptr);
  std::vector<VkQueueFamilyProperties> properties(count);
  vkGetPhysicalDeviceQueueFamilyProperties(handle_, &count, properties.data());
  
  queue_families_.reserve(count);
  for (uint32_t i = 0; i < count; ++i) {

    queue_families_.emplace_back(queue_family{*this, i, properties[i].queueCount,
                                              properties[i].queueFlags});
  }

  // Get the memory properties for the device.
  vkGetPhysicalDeviceMemoryProperties(handle_, &memory_properties_);
  for (auto i = 0u; i < memory_properties_.memoryTypeCount; ++i) {
    auto &memory_type = memory_properties_.memoryTypes[i];
    memory_types_.emplace_back(i, memory_type,
                       memory_properties_.memoryHeaps[memory_type.heapIndex]);
  }
}

physical_device::memory_type_range physical_device::memory_types() const {
  return memory_type_range{
    memory_types_.begin(), memory_types_.end()
  };
}

physical_device::queue_family_range physical_device::queue_families() const {
  return queue_family_range{
    queue_families_.begin(), queue_families_.end()
  };
}

std::vector<surface_format> physical_device::surface_formats(surface surface) const {
  std::vector<surface_format> surface_formats;
  
  uint32_t count = 0;
  auto result = vkGetPhysicalDeviceSurfaceFormatsKHR(*this, surface, &count, nullptr);
  if (VK_SUCCESS == result) {
    std::vector<VkSurfaceFormatKHR> formats(count);
    auto result = vkGetPhysicalDeviceSurfaceFormatsKHR(*this, surface, &count, formats.data());
    if (VK_SUCCESS == result) {
      for (auto &fmt: formats) {
        // We relying on the Vulkan enums being bitwise matches to ours.
        surface_format tmp{static_cast<texel_format>(fmt.format)};
        surface_formats.emplace_back(tmp);
      }
    }
  }

  return surface_formats;
}

