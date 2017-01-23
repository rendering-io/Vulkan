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
class command_builder;
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

class memory_barrier;
class buffer_memory_barrier;
class image_memory_barrier;

enum class image_layout {
  undefined                = VK_IMAGE_LAYOUT_UNDEFINED,
  general                  = VK_IMAGE_LAYOUT_GENERAL,
  color_attachment         = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
  depth_stencil_attachment = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
  depth_stencil_readonly   = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
  shader_readonly          = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
  transfer_source          = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
  transfer_destination     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
  preinitialized           = VK_IMAGE_LAYOUT_PREINITIALIZED,
  present_source           = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
};

enum class texel_format: uint32_t {
  undefined = VK_FORMAT_UNDEFINED,
  r4g4_unorm_pack8 = VK_FORMAT_R4G4_UNORM_PACK8,
  r4g4b4a4_unorm_pack16 = VK_FORMAT_R4G4B4A4_UNORM_PACK16,
  b4g4r4a4_unorm_pack16 = VK_FORMAT_B4G4R4A4_UNORM_PACK16,
  r5g6b5_unorm_pack16 = VK_FORMAT_R5G6B5_UNORM_PACK16,
  b5g6r5_unorm_pack16 = VK_FORMAT_B5G6R5_UNORM_PACK16,
  r5g5b5a1_unorm_pack16 = VK_FORMAT_R5G5B5A1_UNORM_PACK16,
  b5g5r5a1_unorm_pack16 = VK_FORMAT_B5G5R5A1_UNORM_PACK16,
  a1r5g5b5_unorm_pack16 = VK_FORMAT_A1R5G5B5_UNORM_PACK16,
  r8_unorm = VK_FORMAT_R8_UNORM,
  r8_snorm = VK_FORMAT_R8_SNORM,
  r8_uscaled = VK_FORMAT_R8_USCALED,
  r8_sscaled = VK_FORMAT_R8_SSCALED,
  r8_uint = VK_FORMAT_R8_UINT,
  r8_sint = VK_FORMAT_R8_SINT,
  r8_srgb = VK_FORMAT_R8_SRGB,
  r8g8_unorm = VK_FORMAT_R8G8_UNORM,
  r8g8_snorm = VK_FORMAT_R8G8_SNORM,
  r8g8_uscaled = VK_FORMAT_R8G8_USCALED,
  r8g8_sscaled = VK_FORMAT_R8G8_SSCALED,
  r8g8_uint = VK_FORMAT_R8G8_UINT,
  r8g8_sint = VK_FORMAT_R8G8_SINT,
  r8g8_srgb = VK_FORMAT_R8G8_SRGB,
  r8g8b8_unorm = VK_FORMAT_R8G8B8_UNORM,
  r8g8b8_snorm = VK_FORMAT_R8G8B8_SNORM,
  r8g8b8_uscaled = VK_FORMAT_R8G8B8_USCALED,
  r8g8b8_sscaled = VK_FORMAT_R8G8B8_SSCALED,
  r8g8b8_uint = VK_FORMAT_R8G8B8_UINT,
  r8g8b8_sint = VK_FORMAT_R8G8B8_SINT,
  r8g8b8_srgb = VK_FORMAT_R8G8B8_SRGB,
  b8g8r8_unorm = VK_FORMAT_B8G8R8_UNORM,
  b8g8r8_snorm = VK_FORMAT_B8G8R8_SNORM,
  b8g8r8_uscaled = VK_FORMAT_B8G8R8_USCALED,
  b8g8r8_sscaled = VK_FORMAT_B8G8R8_SSCALED,
  b8g8r8_uint = VK_FORMAT_B8G8R8_UINT,
  b8g8r8_sint = VK_FORMAT_B8G8R8_SINT,
  b8g8r8_srgb = VK_FORMAT_B8G8R8_SRGB,
  r8g8b8a8_unorm = VK_FORMAT_R8G8B8A8_UNORM,
  r8g8b8a8_snorm = VK_FORMAT_R8G8B8A8_SNORM,
  r8g8b8a8_uscaled = VK_FORMAT_R8G8B8A8_USCALED,
  r8g8b8a8_sscaled = VK_FORMAT_R8G8B8A8_SSCALED,
  r8g8b8a8_uint = VK_FORMAT_R8G8B8A8_UINT,
  r8g8b8a8_sint = VK_FORMAT_R8G8B8A8_SINT,
  r8g8b8a8_srgb = VK_FORMAT_R8G8B8A8_SRGB,
  b8g8r8a8_unorm = VK_FORMAT_B8G8R8A8_UNORM,
  b8g8r8a8_snorm = VK_FORMAT_B8G8R8A8_SNORM,
  b8g8r8a8_uscaled = VK_FORMAT_B8G8R8A8_USCALED,
  b8g8r8a8_sscaled = VK_FORMAT_B8G8R8A8_SSCALED,
  b8g8r8a8_uint = VK_FORMAT_B8G8R8A8_UINT,
  b8g8r8a8_sint = VK_FORMAT_B8G8R8A8_SINT,
  b8g8r8a8_srgb = VK_FORMAT_B8G8R8A8_SRGB,
  a8b8g8r8_unorm_pack32 = VK_FORMAT_A8B8G8R8_UNORM_PACK32,
  a8b8g8r8_snorm_pack32 = VK_FORMAT_A8B8G8R8_SNORM_PACK32,
  a8b8g8r8_uscaled_pack32 = VK_FORMAT_A8B8G8R8_USCALED_PACK32,
  a8b8g8r8_sscaled_pack32 = VK_FORMAT_A8B8G8R8_SSCALED_PACK32,
  a8b8g8r8_uint_pack32 = VK_FORMAT_A8B8G8R8_UINT_PACK32,
  a8b8g8r8_sint_pack32 = VK_FORMAT_A8B8G8R8_SINT_PACK32,
  a8b8g8r8_srgb_pack32 = VK_FORMAT_A8B8G8R8_SRGB_PACK32,
  a2r10g10b10_unorm_pack32 = VK_FORMAT_A2R10G10B10_UNORM_PACK32,
  a2r10g10b10_snorm_pack32 = VK_FORMAT_A2R10G10B10_SNORM_PACK32,
  a2r10g10b10_uscaled_pack32 = VK_FORMAT_A2R10G10B10_USCALED_PACK32,
  a2r10g10b10_sscaled_pack32 = VK_FORMAT_A2R10G10B10_SSCALED_PACK32,
  a2r10g10b10_uint_pack32 = VK_FORMAT_A2R10G10B10_UINT_PACK32,
  a2r10g10b10_sint_pack32 = VK_FORMAT_A2R10G10B10_SINT_PACK32,
  a2b10g10r10_unorm_pack32 = VK_FORMAT_A2B10G10R10_UNORM_PACK32,
  a2b10g10r10_snorm_pack32 = VK_FORMAT_A2B10G10R10_SNORM_PACK32,
  a2b10g10r10_uscaled_pack32 = VK_FORMAT_A2B10G10R10_USCALED_PACK32,
  a2b10g10r10_sscaled_pack32 = VK_FORMAT_A2B10G10R10_SSCALED_PACK32,
  a2b10g10r10_uint_pack32 = VK_FORMAT_A2B10G10R10_UINT_PACK32,
  a2b10g10r10_sint_pack32 = VK_FORMAT_A2B10G10R10_SINT_PACK32,
  r16_unorm = VK_FORMAT_R16_UNORM,
  r16_snorm = VK_FORMAT_R16_SNORM,
  r16_uscaled = VK_FORMAT_R16_USCALED,
  r16_sscaled = VK_FORMAT_R16_SSCALED,
  r16_uint = VK_FORMAT_R16_UINT,
  r16_sint = VK_FORMAT_R16_SINT,
  r16_sfloat = VK_FORMAT_R16_SFLOAT,
  r16g16_unorm = VK_FORMAT_R16G16_UNORM,
  r16g16_snorm = VK_FORMAT_R16G16_SNORM,
  r16g16_uscaled = VK_FORMAT_R16G16_USCALED,
  r16g16_sscaled = VK_FORMAT_R16G16_SSCALED,
  r16g16_uint = VK_FORMAT_R16G16_UINT,
  r16g16_sint = VK_FORMAT_R16G16_SINT,
  r16g16_sfloat = VK_FORMAT_R16G16_SFLOAT,
  r16g16b16_unorm = VK_FORMAT_R16G16B16_UNORM,
  r16g16b16_snorm = VK_FORMAT_R16G16B16_SNORM,
  r16g16b16_uscaled = VK_FORMAT_R16G16B16_USCALED,
  r16g16b16_sscaled = VK_FORMAT_R16G16B16_SSCALED,
  r16g16b16_uint = VK_FORMAT_R16G16B16_UINT,
  r16g16b16_sint = VK_FORMAT_R16G16B16_SINT,
  r16g16b16_sfloat = VK_FORMAT_R16G16B16_SFLOAT,
  r16g16b16a16_unorm = VK_FORMAT_R16G16B16A16_UNORM,
  r16g16b16a16_snorm = VK_FORMAT_R16G16B16A16_SNORM,
  r16g16b16a16_uscaled = VK_FORMAT_R16G16B16A16_USCALED,
  r16g16b16a16_sscaled = VK_FORMAT_R16G16B16A16_SSCALED,
  r16g16b16a16_uint = VK_FORMAT_R16G16B16A16_UINT,
  r16g16b16a16_sint = VK_FORMAT_R16G16B16A16_SINT,
  r16g16b16a16_sfloat = VK_FORMAT_R16G16B16A16_SFLOAT,
  r32_uint = VK_FORMAT_R32_UINT,
  r32_sint = VK_FORMAT_R32_SINT,
  r32_sfloat = VK_FORMAT_R32_SFLOAT,
  r32g32_uint = VK_FORMAT_R32G32_UINT,
  r32g32_sint = VK_FORMAT_R32G32_SINT,
  r32g32_sfloat = VK_FORMAT_R32G32_SFLOAT,
  r32g32b32_uint = VK_FORMAT_R32G32B32_UINT,
  r32g32b32_sint = VK_FORMAT_R32G32B32_SINT,
  r32g32b32_sfloat = VK_FORMAT_R32G32B32_SFLOAT,
  r32g32b32a32_uint = VK_FORMAT_R32G32B32A32_UINT,
  r32g32b32a32_sint = VK_FORMAT_R32G32B32A32_SINT,
  r32g32b32a32_sfloat = VK_FORMAT_R32G32B32A32_SFLOAT,
  r64_uint = VK_FORMAT_R64_UINT,
  r64_sint = VK_FORMAT_R64_SINT,
  r64_sfloat = VK_FORMAT_R64_SFLOAT,
  r64g64_uint = VK_FORMAT_R64G64_UINT,
  r64g64_sint = VK_FORMAT_R64G64_SINT,
  r64g64_sfloat = VK_FORMAT_R64G64_SFLOAT,
  r64g64b64_uint = VK_FORMAT_R64G64B64_UINT,
  r64g64b64_sint = VK_FORMAT_R64G64B64_SINT,
  r64g64b64_sfloat = VK_FORMAT_R64G64B64_SFLOAT,
  r64g64b64a64_uint = VK_FORMAT_R64G64B64A64_UINT,
  r64g64b64a64_sint = VK_FORMAT_R64G64B64A64_SINT,
  r64g64b64a64_sfloat = VK_FORMAT_R64G64B64A64_SFLOAT,
  b10g11r11_ufloat_pack32 = VK_FORMAT_B10G11R11_UFLOAT_PACK32,
  e5b9g9r9_ufloat_pack32 = VK_FORMAT_E5B9G9R9_UFLOAT_PACK32,
  d16_unorm = VK_FORMAT_D16_UNORM,
  x8_d24_unorm_pack32 = VK_FORMAT_X8_D24_UNORM_PACK32,
  d32_sfloat = VK_FORMAT_D32_SFLOAT,
  s8_uint = VK_FORMAT_S8_UINT,
  d16_unorm_s8_uint = VK_FORMAT_D16_UNORM_S8_UINT,
  d24_unorm_s8_uint = VK_FORMAT_D24_UNORM_S8_UINT,
  d32_sfloat_s8_uint = VK_FORMAT_D32_SFLOAT_S8_UINT,
  bc1_rgb_unorm_block = VK_FORMAT_BC1_RGB_UNORM_BLOCK,
  bc1_rgb_srgb_block = VK_FORMAT_BC1_RGB_SRGB_BLOCK,
  bc1_rgba_unorm_block = VK_FORMAT_BC1_RGBA_UNORM_BLOCK,
  bc1_rgba_srgb_block = VK_FORMAT_BC1_RGBA_SRGB_BLOCK,
  bc2_unorm_block = VK_FORMAT_BC2_UNORM_BLOCK,
  bc2_srgb_block = VK_FORMAT_BC2_SRGB_BLOCK,
  bc3_unorm_block = VK_FORMAT_BC3_UNORM_BLOCK,
  bc3_srgb_block = VK_FORMAT_BC3_SRGB_BLOCK,
  bc4_unorm_block = VK_FORMAT_BC4_UNORM_BLOCK,
  bc4_snorm_block = VK_FORMAT_BC4_SNORM_BLOCK,
  bc5_unorm_block = VK_FORMAT_BC5_UNORM_BLOCK,
  bc5_snorm_block = VK_FORMAT_BC5_SNORM_BLOCK,
  bc6h_ufloat_block = VK_FORMAT_BC6H_UFLOAT_BLOCK,
  bc6h_sfloat_block = VK_FORMAT_BC6H_SFLOAT_BLOCK,
  bc7_unorm_block = VK_FORMAT_BC7_UNORM_BLOCK,
  bc7_srgb_block = VK_FORMAT_BC7_SRGB_BLOCK,
  etc2_r8g8b8_unorm_block = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK,
  etc2_r8g8b8_srgb_block = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK,
  etc2_r8g8b8a1_unorm_block = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK,
  etc2_r8g8b8a1_srgb_block = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK,
  etc2_r8g8b8a8_unorm_block = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK,
  etc2_r8g8b8a8_srgb_block = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK,
  eac_r11_unorm_block = VK_FORMAT_EAC_R11_UNORM_BLOCK,
  eac_r11_snorm_block = VK_FORMAT_EAC_R11_SNORM_BLOCK,
  eac_r11g11_unorm_block = VK_FORMAT_EAC_R11G11_UNORM_BLOCK,
  eac_r11g11_snorm_block = VK_FORMAT_EAC_R11G11_SNORM_BLOCK,
  astc_4x4_unorm_block = VK_FORMAT_ASTC_4x4_UNORM_BLOCK,
  astc_4x4_srgb_block = VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
  astc_5x4_unorm_block = VK_FORMAT_ASTC_5x4_UNORM_BLOCK,
  astc_5x4_srgb_block = VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
  astc_5x5_unorm_block = VK_FORMAT_ASTC_5x5_UNORM_BLOCK,
  astc_5x5_srgb_block = VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
  astc_6x5_unorm_block = VK_FORMAT_ASTC_6x5_UNORM_BLOCK,
  astc_6x5_srgb_block = VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
  astc_6x6_unorm_block = VK_FORMAT_ASTC_6x6_UNORM_BLOCK,
  astc_6x6_srgb_block = VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
  astc_8x5_unorm_block = VK_FORMAT_ASTC_8x5_UNORM_BLOCK,
  astc_8x5_srgb_block = VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
  astc_8x6_unorm_block = VK_FORMAT_ASTC_8x6_UNORM_BLOCK,
  astc_8x6_srgb_block = VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
  astc_8x8_unorm_block = VK_FORMAT_ASTC_8x8_UNORM_BLOCK,
  astc_8x8_srgb_block = VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
  astc_10x5_unorm_block = VK_FORMAT_ASTC_10x5_UNORM_BLOCK,
  astc_10x5_srgb_block = VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
  astc_10x6_unorm_block = VK_FORMAT_ASTC_10x6_UNORM_BLOCK,
  astc_10x6_srgb_block = VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
  astc_10x8_unorm_block = VK_FORMAT_ASTC_10x8_UNORM_BLOCK,
  astc_10x8_srgb_block = VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
  astc_10x10_unorm_block = VK_FORMAT_ASTC_10x10_UNORM_BLOCK,
  astc_10x10_srgb_block = VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
  astc_12x10_unorm_block = VK_FORMAT_ASTC_12x10_UNORM_BLOCK,
  astc_12x10_srgb_block = VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
  astc_12x12_unorm_block = VK_FORMAT_ASTC_12x12_UNORM_BLOCK,
  astc_12x12_srgb_block = VK_FORMAT_ASTC_12x12_SRGB_BLOCK,
/*  
  pvrtc1_2bpp_unorm_block_img = VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG,
  pvrtc1_4bpp_unorm_block_img = VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG,
  pvrtc2_2bpp_unorm_block_img = VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG,
  pvrtc2_4bpp_unorm_block_img = VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG,
  pvrtc1_2bpp_srgb_block_img = VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG,
  pvrtc1_4bpp_srgb_block_img = VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG,
  pvrtc2_2bpp_srgb_block_img = VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG,
  pvrtc2_4bpp_srgb_block_img = VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG,
*/
};

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

struct viewport {
  float x;
  float y;
  float width;
  float height;
  float min_depth;
  float max_depth;
};

struct surface_format {
  texel_format format;
};

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

  bool is_surface_supported(surface surface) const;
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

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  bool is_presentation_supported() const;
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
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
  std::vector<surface_format> surface_formats(surface surface) const;
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

  template<typename F>
  void record(F f) {
    command_builder builder{*this};
    begin();
    f(builder);
    end();
  }
private:
  class impl;
  std::shared_ptr<impl> impl_;

  friend class command_pool;
};

using stage_mask = uint32_t;
 
class command_builder {
private:
  command_builder(command_buffer &buffer);

public:
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
  void end_query(query_pool pool, uint32_t index);
  void end_render_pass();
  void execute_commands(command_buffer* buffers, uint32_t buffer_count);
  void fill_buffer(buffer buffer, size_t offset, uint32_t value, ssize_t size);
  void pipeline_barrier(const memory_barrier *barriers,
                        uint32_t barrier_count,
                        const buffer_memory_barrier * buffer_barriers,
                        uint32_t buffer_barrier_count,
                        const image_memory_barrier *image_barriers,
                        uint32_t image_barrier_count, image image);
  void set_line_width(float width);
  void set_depth_bias(float constant_factor, float clamp, float slope_factor);
  void set_depth_bounds(float min, float max);
  void set_event(event event, stage_mask mask);
  void set_viewports(viewport *viewports, size_t viewport_count);
private:
  command_buffer &buffer_;

  friend class command_buffer;
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

  size_t get_commitment() const;
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

class image_subresource {
  
};

class image_subresource_range {
  
};

class image {
protected:
  image(device device, VkImage handle, bool owns_handle);

public:
  void bind(device_memory memory, size_t offset, size_t size);
  size_t minimum_allocation_size() const;
  size_t minimum_allocation_alignment() const;

  operator VkImage();
private:
  class impl;
  std::shared_ptr<impl> impl_;
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

  operator VkQueryPool();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class buffer_view {
public:
  buffer_view(device device, buffer buffer, texel_format format, size_t offset, size_t range);

private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class image_view {
public:
  image_view(device device);

  operator VkImageView();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class pipeline_cache {
public:
  pipeline_cache(device device, const void *data, size_t size_in_bytes);

  operator VkPipelineCache();

  void merge(pipeline_cache *caches, size_t cache_count);
  size_t size() const;
  std::vector<uint8_t> data() const;
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

  operator VkRenderPass();
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
public:
  graphics_pipeline(device device, pipeline_layout layout, 
                    render_pass render_pass);
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
  framebuffer(device device, render_pass pass,
              image_view *attachments, size_t attachment_count,
              uint32_t width, uint32_t height, uint32_t layers);

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
#ifdef VK_USE_PLATFORM_XLIB_KHR
  surface(instance instance, Display *display, Window window);
#endif 

#ifdef VK_USE_PLATFORM_XCB_KHR
  surface(instance instance, xcb_connection_t* connection, xcb_window_t window);
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
  surface(instance instance, MirConnection *connection, MirSurface *surface);
#endif 

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
  surface(instance instance, wl_display *display, wl_surface *surface);
#endif 

#ifdef VK_USE_PLATFORM_ANDROID_KHR
  surface(instance instance, ANativeWindow *window);
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
  surface(instance instance, HINSTANCE instance, HWND window);
#endif 

  operator VkSurfaceKHR();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class swapchain {
public:
  swapchain(device device, surface surface, surface_format format);

  operator VkSwapchainKHR();

  swapchain_image acquire_next_image();
private:
  class impl;
  std::shared_ptr<impl> impl_;
};

class swapchain_image : public image {
private:
  swapchain_image(device device, swapchain chain, VkImage handle, uint32_t index); 
  
  swapchain swapchain_;
  uint32_t index_;

  friend class queue;
  friend class swapchain;
};

class display {};
class display_mode {};

}

#endif // ifndef VK_VK_H

