# Build the vk library.
add_library(vk buffer.c++
               buffer_view.c++
               command_buffer.c++
               command_pool.c++
               descriptor_pool.c++
               descriptor_set_layout.c++
               device.c++
               device_memory.c++
	       event.c++
	       fence.c++
               framebuffer.c++
	       image.c++
               image_view.c++
               instance.c++
               physical_device.c++
               pipeline.c++
               pipeline_cache.c++
               pipeline_layout.c++
               queue.c++
               query_pool.c++
               render_pass.c++
               sampler.c++
               semaphore.c++
               shader_module.c++
               surface.c++
               swapchain.c++)
target_link_libraries(vk PUBLIC vulkan)

