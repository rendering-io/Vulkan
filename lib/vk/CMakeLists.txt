# Build the vk library.
add_library(vk buffer.c++
               command_buffer.c++
               command_pool.c++
               descriptor_pool.c++
               descriptor_set_layout.c++
               device.c++
               device_memory.c++
               instance.c++
               physical_device.c++
               pipeline.c++
               pipeline_layout.c++
               queue.c++
               shader_module.c++
               surface.c++
               swapchain.c++)
target_link_libraries(vk PUBLIC vulkan)

