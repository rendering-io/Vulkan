#include <vk/vk.h>
#include <cassert>
#include <fstream>
#include <iostream>

std::vector<uint32_t> load_shader(const char *path) {
  std::ifstream in(path, std::ios::in | std::ios::binary);
  if (in) {
    std::vector<uint32_t> buf;
    in.seekg(0, std::ios::end);

    auto size_in_bytes = in.tellg();
    assert(0 == size_in_bytes % sizeof(uint32_t));
    
    buf.resize(in.tellg() / sizeof(uint32_t));
    in.seekg(0, std::ios::beg);
    in.read(reinterpret_cast<char*>(buf.data()), size_in_bytes);
    in.close();
    return buf;
  }
  throw(errno);
}

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
  const vk::queue_family *family = nullptr;
  for (auto &queue_family: best_physical_device->queue_families()) {
    std::cout << "Queue family " << queue_family.index << "\n";
    family = &queue_family;
    break;
  }

  // Now we can create a logical device.
  vk::device device{*best_physical_device};
  vk::queue queue = device.get_queue(family->index, 0);

  // Now we want to find a host-coherent memory region.
  const vk::physical_device::memory_type* best_memory_type = nullptr;
  for (auto &memory_type: best_physical_device->memory_types()) {
    if (memory_type.is_host_coherent())
      best_memory_type = &memory_type;
  }

  if (nullptr == best_memory_type) {
    std::cerr << "Unable to find a host-coherent memory type. Exiting...";
    return -1;
  }

  // Create three buffers,
  vk::buffer a{device, 1024};
  vk::buffer b{device, 1024};
  vk::buffer c{device, 1024};

  // We should be calling get_memory_requirements for each buffer here.
  vk::device_memory storage{device, *best_memory_type, 3 * 1024};
  a.bind_memory(storage, 0, 1024);
  b.bind_memory(storage, 1024, 1024);
  c.bind_memory(storage, 2048, 1024);

  // Map the range of buffer b.
  void *ptr = nullptr;
  if (map_memory(storage, 1024, 1024, &ptr)) {
    unmap_memory(storage);
  }

  if (map_memory(storage, 2048, 1024, &ptr)) {
    unmap_memory(storage);
  }

  // Load the shader module.
  auto spirv = load_shader("vector_add.spv");
  vk::shader_module shader{device, spirv.data(), spirv.size() * sizeof(uint32_t)};

  // Define some layout bindings.
  std::vector<vk::descriptor_set_layout_binding> bindings;
  bindings.emplace_back(0);
  bindings.emplace_back(1);
  bindings.emplace_back(2);

  vk::descriptor_set_layout layout{device, bindings.data(), bindings.size()};
  vk::pipeline_layout pipeline_layout{device, &layout, 1}; 

  // Now construct a pipeline.
  vk::pipeline pipeline{device, pipeline_layout, shader, "main"};

  // While we have defined the layout of our descriptor set, we haven't 
  // actually bound the buffers into a concrete descriptor. Now we create
  // the real descriptor.
  vk::descriptor_pool descriptor_pool{device, 1};

  // Wait for the queue to be idle.
  queue.wait_idle();
  std::cout << "Done waiting for idle.\n";

  return 0;
}

