#include <vk/vk.h>

using namespace vk;

class sampler::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkSampler handle_;
};

sampler::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

sampler::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroySampler(device_, handle_, nullptr);
  }
}

sampler::sampler(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }
