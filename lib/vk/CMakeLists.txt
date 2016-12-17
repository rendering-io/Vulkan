# Build the vk library.
add_library(vk buffer.c++
               descriptor_set_layout.c++
               device.c++
               instance.c++
               physical_device.c++
               pipeline.c++
               pipeline_layout.c++
               queue.c++
               shader_module.c++)
target_link_libraries(vk PUBLIC vulkan)

