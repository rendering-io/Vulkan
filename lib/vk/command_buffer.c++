#include <vk/vk.h>
#include <cassert>

using namespace vk;

class command_buffer::impl {
public:
  impl(device device, command_pool pool, VkCommandBuffer handle);
  ~impl();

  device device_;
  command_pool pool_;
  VkCommandBuffer handle_;
};

command_buffer::impl::impl(device device, command_pool pool, VkCommandBuffer handle)
: device_{device}, pool_{pool}, handle_{handle} { }

command_buffer::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkFreeCommandBuffers(device_, pool_, 1, &handle_);
  }
}

command_buffer::command_buffer(device device, command_pool pool, VkCommandBuffer handle)
: impl_{std::make_shared<impl>(device, pool, handle)} {}

void command_buffer::begin() {
  VkCommandBufferBeginInfo info;
  info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  info.pNext = nullptr;
  info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  info.pInheritanceInfo = nullptr;
  auto result = vkBeginCommandBuffer(impl_->handle_, &info);
  assert(VK_SUCCESS == result && "Error starting command buffer.");
}

command_buffer::operator VkCommandBuffer() {
  return impl_->handle_;
}

void command_buffer::bind_descriptor_sets(pipeline_layout layout, 
                                          descriptor_set* descriptors, size_t descriptor_count) {
  std::vector<VkDescriptorSet> sets(descriptor_count);
  for (auto i = 0ul; i < descriptor_count; ++i)
    sets[i] = descriptors[i];

  vkCmdBindDescriptorSets(impl_->handle_, VK_PIPELINE_BIND_POINT_COMPUTE,
                          layout, 0, sets.size(), sets.data(), 0, nullptr);
}

void command_buffer::bind_pipeline(pipeline pipeline) {
  vkCmdBindPipeline(impl_->handle_, VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);
}

void command_buffer::end() {
  auto result = vkEndCommandBuffer(impl_->handle_);
  assert(VK_SUCCESS == result && "Error completing command buffer.");
}

void command_buffer::reset(bool release_all) {
  VkCommandBufferResetFlags flags =
    release_all ? VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT : 0;
  auto result = vkResetCommandBuffer(impl_->handle_, flags);
  assert(VK_SUCCESS == result && "Failed to reset command buffer.");
}

void command_buffer::dispatch(uint32_t x, uint32_t y, uint32_t z) {
  vkCmdDispatch(impl_->handle_, x, y, z);
}

