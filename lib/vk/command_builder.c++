#include <vk/vk.h>
#include <cassert>

using namespace vk;

command_builder::command_builder(command_buffer &buffer)
: buffer_{buffer} { }

void command_builder::bind_index_buffer(buffer buffer, size_t offset, index_type type) {
  VkIndexType vk_index_type;
  switch (type) {
  case index_type::uint16:
    vk_index_type = VK_INDEX_TYPE_UINT16;
    break;

  case index_type::uint32:
    vk_index_type = VK_INDEX_TYPE_UINT32;
    break;

  default:
    assert(false && "Invalid index type.");
  }

  vkCmdBindIndexBuffer(buffer_, buffer, offset, vk_index_type);
}

void command_builder::dispatch(uint32_t x, uint32_t y, uint32_t z) {
  vkCmdDispatch(buffer_, x, y, z);
}

void command_builder::dispatch_indirect(buffer buffer, size_t offset) {
  vkCmdDispatchIndirect(buffer_, buffer, offset);
}

void command_builder::draw(uint32_t vertex_count, uint32_t instance_count,
                           uint32_t first_vertex, uint32_t first_instance) {
  vkCmdDraw(buffer_, vertex_count, instance_count, first_vertex, first_instance);
}

void command_builder::draw_indexed(uint32_t index_count, uint32_t instance_count,
                                   uint32_t first_index, int32_t vertex_offset,
                                   uint32_t first_instance) {
  vkCmdDrawIndexed(buffer_, index_count, instance_count,first_index, vertex_offset, first_instance);
}

void command_builder::draw_indirect(buffer buffer, size_t offset,
                                    uint32_t draw_count, uint32_t stride) {
  vkCmdDrawIndirect(buffer_, buffer, offset, draw_count, stride);
}

void command_builder::draw_indexed_indirect(buffer buffer, size_t offset,
                                            uint32_t draw_count, uint32_t stride) {
  vkCmdDrawIndexedIndirect(buffer_, buffer, offset, draw_count, stride);
}  

void command_builder::end_query(query_pool pool, uint32_t index) {
  vkCmdEndQuery(buffer_, pool, index);
}

void command_builder::end_render_pass() {
  vkCmdEndRenderPass(buffer_);
}

void command_builder::execute_commands(command_buffer* buffers, uint32_t buffer_count) {
  std::vector<VkCommandBuffer> cmd_buffers(buffer_count);
  for (auto i = 0u; i < buffer_count; ++i) {
    cmd_buffers[i] = buffers[i];
  }
  vkCmdExecuteCommands(buffer_, cmd_buffers.size(), cmd_buffers.data());
}

void command_builder::fill_buffer(buffer buffer, size_t offset, uint32_t value, ssize_t size) {
  if (size < 0)
    size = VK_WHOLE_SIZE;

  vkCmdFillBuffer(buffer_, buffer, offset, size, value);
}

void command_builder::pipeline_barrier(const memory_barrier *barriers,
                                       uint32_t barrier_count,
                                       const buffer_memory_barrier *buffer_barriers,
                                       uint32_t buffer_barrier_count,
                                       const image_memory_barrier *image_barriers, 
                                       uint32_t image_barrier_count,image image ) {
  VkPipelineStageFlags src_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  VkPipelineStageFlags dst_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  VkDependencyFlags dependency_flags = 0;
  std::vector<VkMemoryBarrier> barrier_buf;
  std::vector<VkBufferMemoryBarrier> buffer_barrier_buf;
  std::vector<VkImageMemoryBarrier> image_barrier_buf;

  VkImageMemoryBarrier barrier;
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER; 
  barrier.pNext = nullptr;
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = 1;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;

  vkCmdPipelineBarrier(buffer_, src_stage_flags, dst_stage_flags, dependency_flags,
                       barrier_count, barrier_buf.data(),
                       buffer_barrier_count, buffer_barrier_buf.data(),
                       /*image_barrier_count*/1, &barrier/*image_barrier_buf.data()*/);
}

void command_builder::reset_event(event event, pipeline_stage stage_mask) {
  vkCmdResetEvent(buffer_, event, 
                  static_cast<VkPipelineStageFlags>(stage_mask));
}

void command_builder::set_event(event event, pipeline_stage stage_mask) {
  vkCmdSetEvent(buffer_, event, 
                static_cast<VkPipelineStageFlags>(stage_mask));
}

void command_builder::set_line_width(float width) {
  vkCmdSetLineWidth(buffer_, width);
}

void command_builder::set_depth_bias(float constant_factor, float clamp, float slope_factor) {
  vkCmdSetDepthBias(buffer_, constant_factor, clamp, slope_factor);
}

void command_builder::set_depth_bounds(float min, float max) {
  vkCmdSetDepthBounds(buffer_, min, max);
}

void command_builder::set_viewports(viewport *viewports, size_t viewport_count) {
  vkCmdSetViewport(buffer_, 0, viewport_count, reinterpret_cast<VkViewport*>(viewports));
}

void command_builder::update_buffer(buffer dst, size_t offset, const void *src, size_t size) {
#if VK_HEADER_VERSION < 19
  vkCmdUpdateBuffer(buffer_, dst, offset, size, 
                    static_cast<const uint32_t*>(src));
#else
  vkCmdUpdateBuffer(buffer_, dst, offset, size, src);
#endif 
}

