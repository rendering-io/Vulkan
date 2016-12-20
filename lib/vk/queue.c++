#include <vk/vk.h>

using namespace vk;

queue::queue(VkQueue handle) :handle_{handle} {
}

void queue::submit(command_buffer* buffers, size_t buffer_count) { }

void queue::wait_idle() {
  vkQueueWaitIdle(handle_);
}

