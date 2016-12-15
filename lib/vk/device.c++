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
  info.enabledLayerCount = 0;
  info.ppEnabledLayerNames = nullptr;
  info.enabledExtensionCount = 0;
  info.ppEnabledExtensionNames = nullptr;
  info.pEnabledFeatures = nullptr;

  VkDevice handle = 0;
  auto result = vkCreateDevice(physical_dev, &info, nullptr, &handle);
  if (VK_SUCCESS != result)
    return;

  impl_->handle_ = handle;
}

queue device::get_queue(uint32_t queue_family, uint32_t index) {
  VkQueue queue_handle = 0;

  vkGetDeviceQueue(impl_->handle_, queue_family, index, &queue_handle);
  return queue{queue_handle};
}

