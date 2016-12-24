#include <vk/vk.h>
#include <cassert>

using namespace vk;

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

void command_builder::execute_commands(command_buffer* buffers, uint32_t buffer_count) {
  std::vector<VkCommandBuffer> cmd_buffers(buffer_count);
  for (auto i = 0u; i < buffer_count; ++i) {
    cmd_buffers[i] = buffers[i];
  }
  vkCmdExecuteCommands(buffer_, cmd_buffers.size(), cmd_buffers.data());
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

