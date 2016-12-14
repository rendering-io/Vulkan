#include <vk/vk.h>
#include <cassert>
#include <vector>

using namespace vk;

class instance::impl {
public:
  ~impl();  

  VkInstance handle_;
  std::vector<physical_device> physical_devices_;
};

instance::impl::~impl() {
  if (handle_) {
    vkDestroyInstance(handle_, nullptr);
  }
}

instance::instance()
: impl_{std::make_shared<impl>()} {
  impl_->handle_ = 0;

  VkInstanceCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.pApplicationInfo = nullptr;
  info.enabledLayerCount = 0;
  info.ppEnabledLayerNames = nullptr;
  info.enabledExtensionCount = 0;
  info.ppEnabledLayerNames = nullptr;

  VkInstance handle = 0;
  auto result = vkCreateInstance(&info, nullptr, &handle);
  if (VK_SUCCESS != result)
    return;
  
  impl_->handle_ = handle;

  // Enumerate physical devices and populate device list.
  enumerate_physical_devices();
}

void instance::enumerate_physical_devices() {
  assert(0 != impl_->handle_ && "Null instance handle.");
  
  // Clear the existing device list.
  impl_->physical_devices_.clear();

  // Query the number of physical devices.
  uint32_t count = 0;
  auto result = vkEnumeratePhysicalDevices(impl_->handle_, &count, nullptr);
  if (VK_SUCCESS != result) 
    return;
  
  std::vector<VkPhysicalDevice> devices(count);
  result = vkEnumeratePhysicalDevices(impl_->handle_, &count, devices.data());
  if (VK_SUCCESS != result)
    return;
  
  for (auto device: devices) {
    impl_->physical_devices_.emplace_back(std::move(physical_device{device}));
  }
}

iterator_range<instance::physical_device_iterator>
instance::physical_devices() const {
  return iterator_range<physical_device_iterator>{
    impl_->physical_devices_.begin(), impl_->physical_devices_.end()
  };
}
