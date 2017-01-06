#include <vk/vk.h>
#include <cassert>
#include <iostream>

using namespace vk;

class device::impl {
public:
  impl(const vk::physical_device&);
  ~impl();

  const vk::physical_device& physical_dev_;
  VkDevice handle_;
};

device::impl::impl(const vk::physical_device& physical_dev)
: physical_dev_{physical_dev}, handle_{VK_NULL_HANDLE} { }

device::impl::~impl() {
  if (0 != handle_) {
    vkDestroyDevice(handle_, nullptr);
  }
}

device::device(const vk::physical_device& physical_dev)
: impl_{std::make_shared<impl>(physical_dev)} {
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

  // We will also enable all extensions.
  count = 0;
  std::vector<VkExtensionProperties> extensions;
  std::vector<const char*> enabled_extensions;
  result = vkEnumerateDeviceExtensionProperties(physical_dev, nullptr, &count, nullptr);
  if (VK_SUCCESS == result) {
    extensions.resize(count);
    result = vkEnumerateDeviceExtensionProperties(physical_dev, nullptr, &count, extensions.data());
    if (VK_SUCCESS == result) {
      for (auto &extension: extensions) {
        enabled_extensions.push_back(extension.extensionName);
        std::cout << "Enabled device extension " << extension.extensionName << "\n";
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
  info.enabledExtensionCount = enabled_extensions.size();
  info.ppEnabledExtensionNames = enabled_extensions.data();
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

const vk::physical_device& device::physical_device() const {
  return impl_->physical_dev_;
}

void device::wait_idle() {
  auto result = vkDeviceWaitIdle(impl_->handle_);
  assert(VK_SUCCESS == result && "Wait for device idle failed.");
}

