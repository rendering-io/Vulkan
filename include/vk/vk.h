#ifndef VK_VK_H
#define VK_VK_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace vk {

class physical_device;
class queue;

template<typename I>
class iterator_range : std::pair<I, I> {
public:
  using std::pair<I, I>::pair;

  I begin() { return this->first; }
  I end() { return this->second; }
};

class instance {
public:
  using physical_device_iterator = std::vector<physical_device>::const_iterator;
  instance();

  iterator_range<physical_device_iterator> physical_devices() const;
private:
  void enumerate_physical_devices();

  class impl;
  std::shared_ptr<impl> impl_;
};

class queue_family {
private:
  queue_family(uint32_t index, uint32_t count, VkQueueFlags flags);
public:
  bool is_graphics_queue() const;
  bool is_compute_queue() const;
  bool is_transfer_queue() const;
  bool is_present_queue() const;
  const uint32_t index;
  const uint32_t count;
private:
  VkQueueFlags flags_;

  friend class physical_device;
};

class physical_device {
private:
  physical_device(VkPhysicalDevice);

public:
  using queue_family_iterator = std::vector<queue_family>::const_iterator;
  using queue_family_range = iterator_range<queue_family_iterator>;
  
  operator VkPhysicalDevice() const { return handle_; }

  queue_family_range queue_families() const;
private: 
  VkPhysicalDevice handle_;

  // Supported queue families.
  std::vector<queue_family> queue_families_; 
  friend class instance;
};

class device {
public:
  device(const physical_device&);

  queue get_queue(uint32_t family, uint32_t index);
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class queue {
private: 
  queue(VkQueue handle);
public:
  void submit();
  void wait_idle();
private:
  VkQueue handle_;

  friend class device;
};

class shader_module {};

class buffer {
public:
  buffer(instance);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

}

#endif // ifndef VK_VK_H

