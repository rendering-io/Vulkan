#include <vk/vk.h>

using namespace vk;

class pipeline_cache::impl {
public:
  impl(device device);
  ~impl();

  device device_;
  VkPipelineCache handle_;
};

pipeline_cache::impl::impl(device device)
: device_{std::move(device)}, handle_{VK_NULL_HANDLE} { }

pipeline_cache::impl::~impl() {
  if (VK_NULL_HANDLE != handle_) {
    vkDestroyPipelineCache(device_, handle_, nullptr);
  }
}

pipeline_cache::pipeline_cache(device device)
: impl_{std::make_shared<impl>(std::move(device))} { }
