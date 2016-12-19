#include <vk/vk.h>

using namespace vk;

queue_family::queue_family(uint32_t index, uint32_t count, VkQueueFlags flags)
: index{index}, count{count}, flags_{flags}
{ 
}

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

  uint32_t count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(handle_, &count, nullptr);
  std::vector<VkQueueFamilyProperties> properties(count);
  vkGetPhysicalDeviceQueueFamilyProperties(handle_, &count, properties.data());
  
  queue_families_.reserve(count);
  for (uint32_t i = 0; i < count; ++i) {

    queue_families_.emplace_back(queue_family{i, properties[i].queueCount,
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

