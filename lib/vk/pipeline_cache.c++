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

pipeline_cache::operator VkPipelineCache() {
  return impl_->handle_;
}

size_t pipeline_cache::size() const {
  size_t size_in_bytes = 0;
  auto result = vkGetPipelineCacheData(impl_->device_, impl_->handle_, &size_in_bytes, nullptr);
  assert(VK_SUCCESS == result && "Failed to get pipeline cache size.");
  return size_in_bytes;
}

std::vector<uint8_t> pipeline_cache::data() const {
  size_t size_in_bytes = 0;
  auto result = vkGetPipelineCacheData(impl_->device_, impl_->handle_, &size_in_bytes, nullptr);
  if (VK_SUCCESS == result) {
    std::vector<uint8_t> buffer(size_in_bytes);
    result = vkGetPipelineCacheData(impl_->device_, impl_->handle_,
                                    &size_in_bytes, buffer.data());
    if (VK_SUCCESS == result)
      return buffer;
  }

  assert(false && "Failed to get pipeline cache data.");
}

void pipeline_cache::merge(pipeline_cache *caches, size_t cache_count) {
  std::vector<VkPipelineCache> pipeline_caches(cache_count);
  for (auto i = 0ul; i < cache_count; ++i)
    pipeline_caches[i] = caches[i];

  auto result = vkMergePipelineCaches(impl_->device_, impl_->handle_,
                                      cache_count, pipeline_caches.data());
  assert(VK_SUCCESS == result && "Failed to merge pipeline caches.");
}

