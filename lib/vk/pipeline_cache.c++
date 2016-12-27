#include <vk/vk.h>
#include <cassert>

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

pipeline_cache::pipeline_cache(device device, const void* data, size_t size_in_bytes)
: impl_{std::make_shared<impl>(std::move(device))} {
  VkPipelineCacheCreateInfo info;
  info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  info.pNext = nullptr;
  info.flags = 0;
  info.initialDataSize = size_in_bytes;
  info.pInitialData = data;

  auto result = vkCreatePipelineCache(impl_->device_, &info, nullptr, &impl_->handle_);
  assert(VK_SUCCESS == result && "Failed to create pipeline cache.");
}

