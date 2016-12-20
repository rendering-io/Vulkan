#include <vk/vk.h>

using namespace vk;

class device::impl {
public:
  ~impl();

  VkDevice handle_;
};

device::impl::~impl() {
  if (0 != handle_) {
    vkDestroyDevice(handle_, nullptr);
  }
}

device::device(const physical_device& physical_dev)
: impl_{std::make_shared<impl>()} {

  impl_->handle_ = 0;

  // Enumerate the available layers.
  uint32_t count = 0;
  std::vector<VkLayerProperties> layers;
  std::vector<const char*> enabled_layers;
  auto result = vkEnumerateDeviceLayerProperties(physical_dev, &count, nullptr);
  if (VK_SUCCESS == result) {
    layers.resize(count);
    result = vkEnumerateDeviceLayerProperties(physical_dev, &count, layers.data());
    if (VK_SUCCESS == result) {
      for (auto &layer: layers) {
        // We should really check each layer individually, but for now our test 
        // implementation only exposes validation layers.
        enabled_layers.push_back(layer.layerName);
      }
    }
  }
  
  // For now we will just default to the first queue. This is easy but wrong.
  float priority = 1.0f;

  VkDeviceQueueCreateInfo queue_info;
  queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queue_info.pNext = nullptr;
  queue_info.flags = 0;
  queue_info.queueFamilyIndex = 0;
  queue_info.queueCount = 1;
  queue_info.pQueuePriorities = &priority;


  VkDeviceCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.queueCreateInfoCount = 1;
  info.pQueueCreateInfos = &queue_info;
  info.enabledLayerCount = enabled_layers.size();
  info.ppEnabledLayerNames = enabled_layers.data();
  info.enabledExtensionCount = 0;
  info.ppEnabledExtensionNames = nullptr;
  info.pEnabledFeatures = nullptr;

  VkDevice handle = 0;
  result = vkCreateDevice(physical_dev, &info, nullptr, &handle);
  if (VK_SUCCESS != result)
    return;

  impl_->handle_ = handle;
}

device::operator VkDevice() {
  return impl_->handle_;
}

queue device::get_queue(uint32_t queue_family, uint32_t index) {
  VkQueue queue_handle = 0;

  vkGetDeviceQueue(impl_->handle_, queue_family, index, &queue_handle);
  return queue{queue_handle};
}

