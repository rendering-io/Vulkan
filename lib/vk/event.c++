#include <vk/vk.h>
#include <cassert>

using namespace vk;

class event::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkEvent handle_;
};

event::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

event::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyEvent(device_, handle_, nullptr);
  }
}

event::event(device device)
: impl_{std::make_shared<impl>(std::move(device))} {
  VkEventCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_EVENT_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;

  auto result = vkCreateEvent(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create event.");
}

void event::set() {
  auto result = vkSetEvent(impl_->device_, impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to set event.");
}

void event::reset() {
  auto result = vkResetEvent(impl_->device_, impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to reset event.");
}

