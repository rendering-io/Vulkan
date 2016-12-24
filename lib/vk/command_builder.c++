#include <vk/vk.h>

using namespace vk;

void command_builder::set_line_width(float width) {
  vkCmdSetLineWidth(buffer_, width);
}

void command_builder::set_depth_bias(float constant_factor, float clamp, float slope_factor) {
  vkCmdSetDepthBias(buffer_, constant_factor, clamp, slope_factor);
}

