#include <vk/vk.h>

using namespace vk;

queue::queue(VkQueue handle) :handle_{handle} {
}

void queue::submit() { }

void queue::wait_idle() {
  vkQueueWaitIdle(handle_);
}

