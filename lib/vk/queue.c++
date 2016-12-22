#include <vk/vk.h>
#include <cassert>

using namespace vk;

queue::queue(VkQueue handle) :handle_{handle} {
}

void queue::present(swapchain_image image) {
  VkSwapchainKHR swapchain = image.swapchain_;
  uint32_t index = image.index_;

  VkPresentInfoKHR info;
  info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  info.pNext = nullptr;
  info.waitSemaphoreCount = 0;
  info.pWaitSemaphores = nullptr;
  info.swapchainCount = 1;
  info.pSwapchains = &swapchain;
  info.pImageIndices = &index;
  info.pResults = nullptr;

  auto result = vkQueuePresentKHR(handle_, &info);
  assert(VK_SUCCESS == result && "Present failed.");
}

void queue::submit(command_buffer* buffers, size_t buffer_count) {
  std::vector<VkCommandBuffer> command_bufs(buffer_count);
  for (auto i = 0ul; i < buffer_count; ++i)
    command_bufs[i] = buffers[i];

  VkSubmitInfo info;
  info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  info.pNext = nullptr;
  info.waitSemaphoreCount = 0;
  info.pWaitSemaphores = nullptr;
  info.pWaitDstStageMask = nullptr;
  info.commandBufferCount = buffer_count;
  info.pCommandBuffers = command_bufs.data();
  info.signalSemaphoreCount = 0;
  info.pSignalSemaphores = nullptr;
  auto result = vkQueueSubmit(handle_, 1, &info, VK_NULL_HANDLE);
  assert(VK_SUCCESS == result && "Command buffer submission failed.");
}

void queue::wait_idle() {
  vkQueueWaitIdle(handle_);
}

