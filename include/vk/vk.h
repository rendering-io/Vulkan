#ifndef VK_VK_H
#define VK_VK_H

#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

namespace vk {

class instance;
class device;
class physical_device;
class queue;
class shader_module;
class buffer;
class semaphore;
class command_buffer;
class fence;
class device_memory;
class image;
class event;
class query_pool;
class buffer_view;
class image_view;
class pipeline_cache;
class pipeline_layout;
class render_pass;
class pipeline;
class descriptor_set_layout;
class sampler;
class descriptor_pool;
class descriptor_set;
class framebuffer;
class command_pool;
class surface;
class swapchain;
class swapchain_image;
class display;
class display_mode;

enum class index_type {
  uint16,
  uint32
};

enum class signal_status {
  signaled,
  unsignaled
};

// Explicitly binary compatible with VkPipelineStageFlagBits
enum class pipeline_stage {
  top_of_pipe             = 0x00000001,
  draw_indirect           = 0x00000002,
  vertex_input            = 0x00000004,
  vertex_shader           = 0x00000008,
  tessellation_control    = 0x00000010,
  tessellation_evaluation = 0x00000020,
  geometry_shader         = 0x00000040,
  fragment_shader         = 0x00000080,
  early_fragment_tests    = 0x00000100,
  late_fragment_tests     = 0x00000200,
  color_attachment_output = 0x00000400,
  compute_shader          = 0x00000800,
  transfer                = 0x00001000,
  bottom_of_pipe          = 0x00002000,
  host                    = 0x00004000,
  all_graphics            = 0x00008000,
  all_commands            = 0x00010000,
  command_process_nvx     = 0x00020000,
};

inline pipeline_stage operator|(pipeline_stage lhs, pipeline_stage rhs) {
  using T = std::underlying_type_t<pipeline_stage>;
  return static_cast<pipeline_stage>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

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

  operator VkInstance();

  iterator_range<physical_device_iterator> physical_devices() const;
private:
  void enumerate_physical_devices();

  class impl;
  std::shared_ptr<impl> impl_;
};

class queue_family {
private:
  queue_family(physical_device &physical_device, uint32_t index, uint32_t count,
               VkQueueFlags flags);
public:
  bool is_graphics_queue() const;
  bool is_compute_queue() const;
  bool is_transfer_queue() const;
  bool is_present_queue() const;

#ifdef VK_USE_PLATFORM_XLIB_KHR
  bool is_presentation_supported(Display *display, VisualID visual) const;
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
  bool is_presentation_supported(xcb_connection_t *connection, xcb_visualid_t visual) const;
#endif 

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  bool is_presentation_supported(wl_display *display) const;
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
  bool is_presentation_supported(MirConnection *connection) const;
#endif

#ifdef VK_USE_PLATFORM_ANDROID
  bool is_presentation_supported() const;
#endif

#ifdef VK_USE_PLATFORM_WIN32
  bool is_presentation_supported() const;
#endif

  const uint32_t index;
  const uint32_t count;
private:
  VkQueueFlags flags_;
  physical_device &physical_device_;

  friend class physical_device;
};

class physical_device {
private:
  physical_device(VkPhysicalDevice);

public:
  class memory_type;
  using memory_type_iterator  = std::vector<memory_type>::const_iterator;
  using memory_type_range     = iterator_range<memory_type_iterator>;
  using queue_family_iterator = std::vector<queue_family>::const_iterator;
  using queue_family_range    = iterator_range<queue_family_iterator>;
  
  operator VkPhysicalDevice() const { return handle_; }

  memory_type_range memory_types() const;
  queue_family_range queue_families() const;
private: 
  VkPhysicalDevice handle_;

  // Supported queue families.
  std::vector<queue_family> queue_families_;

  // Memory types and heaps.
  VkPhysicalDeviceMemoryProperties memory_properties_;
  std::vector<memory_type> memory_types_;

  VkPhysicalDeviceProperties properties_;
  VkPhysicalDeviceFeatures features_;

  friend class instance;
};

class physical_device::memory_type {
private:
public:
  memory_type(uint32_t index, const VkMemoryType&, const VkMemoryHeap&);

public:
  const uint32_t index;
  bool is_device_local() const;
  bool is_host_visible() const;
  bool is_host_coherent() const;
  bool is_host_cached() const;
  bool is_lazily_allocated() const;

private:
  const VkMemoryType type_;
  const VkMemoryHeap heap_;

  friend class physical_device;
};

class device {
public:
  device(const vk::physical_device&);

  operator VkDevice();
  queue get_queue(uint32_t family, uint32_t index);
  const vk::physical_device& physical_device() const;

  void wait_idle();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class queue {
private: 
  queue(VkQueue handle);
public:
  void submit(command_buffer* buffers, size_t buffer_count);
  void present(swapchain_image image);
  void wait_idle();
private:
  VkQueue handle_;

  friend class device;
};

class shader_module {
public:
  shader_module(device device, const uint32_t* code, size_t size_in_bytes);
  operator VkShaderModule();

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class buffer {
public:
  buffer(device device, size_t size_in_bytes);

  operator VkBuffer();

  void bind(device_memory memory, size_t offset, size_t size);
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class semaphore {
public:
  semaphore(device device);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class command_buffer {
private:
  command_buffer(device device, command_pool pool, VkCommandBuffer handle);

public:
  operator VkCommandBuffer();

  void begin();
  void end();
  void reset(bool release_all);

  void bind_pipeline(pipeline pipeline);
  void bind_descriptor_sets(pipeline_layout layout, descriptor_set* sets, size_t count);
  void dispatch(uint32_t x, uint32_t y = 1, uint32_t z = 1);

private:
  class impl;
  std::shared_ptr<impl> impl_;

  friend class command_pool;
};

using stage_mask = uint32_t;
 
class command_builder {
  void bind_index_buffer(buffer buffer, size_t offset, index_type type);
  void dispatch(uint32_t x, uint32_t y = 1, uint32_t z = 1);
  void dispatch_indirect(buffer buffer, size_t offset = 0);
  void draw(uint32_t vertex_count, uint32_t instance_count,
            uint32_t first_vertex, uint32_t first_instance);
  void draw_indexed(uint32_t index_count, uint32_t instance_count,
                    uint32_t fisrt_index, int32_t vertex_offset, uint32_t first_instance);
  void draw_indirect(buffer buffer, size_t offset,
                     uint32_t draw_count, uint32_t stride);
  void draw_indexed_indirect(buffer buffer, size_t offset,
                             uint32_t draw_count, uint32_t stride);
  void execute_commands(command_buffer* buffers, uint32_t buffer_count);

  void set_line_width(float width);
  void set_depth_bias(float constant_factor, float clamp, float slope_factor);
  void set_depth_bounds(float min, float max);
  void set_event(event event, stage_mask mask);
private:
  command_buffer buffer_;
};

enum class wait_result {
  SUCCESS,
  TIMEOUT
};

class fence {
public:
  fence(device device, bool signaled);

  void reset();
  static void reset(fence *fences, uint32_t fence_count);

  signal_status status() const;
  wait_result wait(uint64_t timeout);
  static wait_result wait_all(fence *fences, uint32_t fence_count,
                              uint64_t timeout) {
    return wait(fences, fence_count, true, timeout);
  }
  static wait_result wait_any(fence *fences, uint32_t fence_count, 
                              uint64_t timeout) {
    return wait(fences, fence_count, false, timeout);
  }

  operator VkFence();

private:
  static wait_result wait(fence *fence, uint32_t fence_count, bool wait_all,
                          uint64_t timeout);

  class impl;
  std::shared_ptr<impl> impl_;
};

class device_memory {
public:
  device_memory(device, const physical_device::memory_type&, size_t); 

public:
  operator VkDeviceMemory();

private:
  class impl;
  std::shared_ptr<impl> impl_;

  friend bool map_memory(device_memory memory, size_t offset, size_t size, void **ptr);
  friend void unmap_memory(device_memory memory);
};

bool map_memory(device_memory, size_t, size_t, void **);
void unmap_memory(device_memory memory);

class image {
private:
  image(device device, VkImage handle, bool owns_handle);

public:
  void bind(device_memory memory, size_t offset, size_t size);
private:
  class impl;
  std::shared_ptr<impl> impl_;

  friend class swapchain;
};

class event {
public:
  event(device device);
  
  signal_status status() const;
  
  void set();
  void reset();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class query_pool {
public:
  query_pool(device device);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class buffer_view {
public:
  buffer_view(device device);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class image_view {
public:
  image_view(device device);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class pipeline_cache {
public:
  pipeline_cache(device device);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class pipeline_layout {
public:
  pipeline_layout(device device, descriptor_set_layout* layouts, 
                  size_t layout_count);
  operator VkPipelineLayout();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class render_pass {
public:
  render_pass(device device);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class pipeline {
protected:
  pipeline(device device);

public:
  operator VkPipeline();

protected:
  class impl;
  std::shared_ptr<impl> impl_;
};

class compute_pipeline: public pipeline {
public:
  compute_pipeline(device device, pipeline_layout layout,
                   shader_module module, const char* entry_point);
};

class graphics_pipeline: public pipeline {
};

class descriptor_set_layout_binding {
public:
  descriptor_set_layout_binding(uint32_t index)
  : binding_index_{index} { }

  uint32_t get_index() const { return binding_index_; }

private:
  uint32_t binding_index_;

};

class descriptor_set_layout {
public:
  descriptor_set_layout(device device, const descriptor_set_layout_binding* bindings,
                        size_t binding_count);

  operator VkDescriptorSetLayout();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class sampler {
public:
  sampler(device device);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class descriptor_pool {
public:
  descriptor_pool(device device, uint32_t max_sets);

  operator VkDescriptorPool();
  descriptor_set allocate(descriptor_set_layout layout);

  void reset();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class descriptor_binding {
public:
  descriptor_binding(uint32_t i, buffer buffer)
  : index{i}, buf{buffer} { }

  uint32_t index;
  buffer buf;
};

class descriptor_set {
private:
  descriptor_set(device device, descriptor_pool pool, VkDescriptorSet);
public:
  operator VkDescriptorSet();

  void update(descriptor_binding* bindings, size_t binding_count);
private:
  class impl;
  std::shared_ptr<impl> impl_;

  friend class descriptor_pool;
};

class framebuffer {
public:
  framebuffer(device device);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class command_pool {
public:
  command_pool(device device, uint32_t queue_family);

  operator VkCommandPool();

  command_buffer allocate();

  void reset(bool release_resources);
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class surface {
public:
#ifdef VK_USE_PLATFORM_XCB_KHR
  surface(instance instance, xcb_connection_t* connection, xcb_window_t window);
#endif // ifdef VK_USE_PLATFORM_XCB_KHR

  operator VkSurfaceKHR();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class swapchain {
public:
  swapchain(device device, surface surface);

  operator VkSwapchainKHR();

  swapchain_image acquire_next_image();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class swapchain_image {
private:
  swapchain_image(swapchain chain, uint32_t index); 
  
  swapchain swapchain_;
  uint32_t index_;

  friend class queue;
  friend class swapchain;
};

class display {};
class display_mode {};

}

#endif // ifndef VK_VK_H

