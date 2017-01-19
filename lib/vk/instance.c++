#include <vk/vk.h>
#include <cassert>
#include <iostream>
#include <vector>

using namespace vk;

class instance::impl {
public:
  impl();
  ~impl();  

  VkInstance handle_;
  VkDebugReportCallbackEXT debug_report_handle_;
  std::vector<physical_device> physical_devices_;
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report_callback(VkDebugReportFlagsEXT flags,
                                                            VkDebugReportObjectTypeEXT obj_type,
                                                            uint64_t object, size_t location,
                                                            int32_t code,
                                                            const char *layer_prefix,
                                                            const char *msg, void *user_data) {
  if (VK_DEBUG_REPORT_INFORMATION_BIT_EXT & flags)
    return VK_FALSE;

  if (VK_DEBUG_REPORT_DEBUG_BIT_EXT & flags)
    return VK_FALSE;

  std::cout << msg << '\n';

  if (VK_DEBUG_REPORT_ERROR_BIT_EXT &flags)
    exit(-1);

  return VK_FALSE;
}

        
instance::impl::impl()
: handle_{VK_NULL_HANDLE}, debug_report_handle_{VK_NULL_HANDLE} {}

instance::impl::~impl() {
  // TODO: This seems a bit suspect. This destroys validation when the instance
  // is destroyed. We probably want debug reporting to persist, but it requires
  // the instance handle to destroy.
  if (debug_report_handle_) {
    auto vkDestroyDebugReportCallbackEXT =
      reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
        vkGetInstanceProcAddr(handle_, "vkDestroyDebugReportCallbackEXT"));
    if (vkDestroyDebugReportCallbackEXT) {
      vkDestroyDebugReportCallbackEXT(handle_, debug_report_handle_, nullptr);
    }
  }

  if (handle_) {
    vkDestroyInstance(handle_, nullptr);
  }
}

instance::instance()
: impl_{std::make_shared<impl>()} {
  // Enumerate the available layers.
  uint32_t count = 0;
  std::vector<VkLayerProperties> layers;
  std::vector<const char*> enabled_layers;
  auto result = vkEnumerateInstanceLayerProperties(&count, nullptr);
  if (VK_SUCCESS == result) {
    layers.resize(count);
    result = vkEnumerateInstanceLayerProperties(&count, layers.data());
    if (VK_SUCCESS == result) {
      for (auto &layer: layers) {
        // We should really check each layer individually, but for now our test
        // implementation only exposes validation layers.
        enabled_layers.push_back(layer.layerName);
      }
    }
  }

  // Enumerate available extensions.
  std::vector<VkExtensionProperties> extensions;
  std::vector<const char*> enabled_extensions;
  count = 0;
  result = vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
  if (VK_SUCCESS == result) {
    extensions.resize(count);
    result = vkEnumerateInstanceExtensionProperties(nullptr, &count, extensions.data());
    if (VK_SUCCESS == result) {
      for (auto &extension: extensions) {
        // We should also check each returned extension, but for now we need
        // all of them.
        enabled_extensions.push_back(extension.extensionName);
        std::cout << "Enabled " << extension.extensionName << "\n";
      }
    }
  }

  VkInstanceCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.pApplicationInfo = nullptr;
  info.enabledLayerCount = enabled_layers.size();
  info.ppEnabledLayerNames = enabled_layers.data();
  info.enabledExtensionCount = enabled_extensions.size();
  info.ppEnabledExtensionNames = enabled_extensions.data();

  VkInstance handle = 0;
  result = vkCreateInstance(&info, nullptr, &handle);
  if (VK_SUCCESS != result)
    return;
  
  impl_->handle_ = handle;

  // Enable the debug callback.
  auto vkCreateDebugReportCallbackEXT =
    reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
        vkGetInstanceProcAddr(impl_->handle_, "vkCreateDebugReportCallbackEXT"));
  if (vkCreateDebugReportCallbackEXT) {
    VkDebugReportCallbackCreateInfoEXT debug_info;
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    debug_info.pNext = nullptr;
    debug_info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                       VK_DEBUG_REPORT_WARNING_BIT_EXT |
                       VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                       VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
                       VK_DEBUG_REPORT_DEBUG_BIT_EXT;
    debug_info.pfnCallback = &debug_report_callback;
    debug_info.pUserData = nullptr;

    result = vkCreateDebugReportCallbackEXT(impl_->handle_, &debug_info,
                                            nullptr,
                                            &impl_->debug_report_handle_);
  }
  // Enumerate physical devices and populate device list.
  enumerate_physical_devices();
}

instance::operator VkInstance() {
  return impl_->handle_;
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
