#include <vk/vk.h>

using namespace vk;

queue_family::queue_family(uint32_t index, uint32_t count, VkQueueFlags flags)
: index{index}, count{count}, flags_{flags}
{ 
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
}

physical_device::queue_family_range physical_device::queue_families() const {
  return queue_family_range{
    queue_families_.begin(), queue_families_.end()
  };
}

