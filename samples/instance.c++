#include <vk/vk.h>
#include <iostream>

int main(int argc, char **argv) {
  // Create a Vulkan instance.
  vk::instance instance;

  // Iterate over the available devices are choose the best one.
  const vk::physical_device *best_physical_device = nullptr;
  for (auto &device: instance.physical_devices()) {
    best_physical_device = &device;
  }
 
  if (nullptr == best_physical_device) {
    std::cerr << "No Vulkan compatible devices found. Exiting...\n";
    return -1;
  }

  // We now need to query the device for supported queue familes and decide
  // what queues we need to construct.
  
  for (auto &queue_family: best_physical_device->queue_families()) {
    std::cout << "Queue family " << queue_family.index << "\n";
  }

  // Now we can create a logical device.
  vk::device device{*best_physical_device};
  return 0;
};

