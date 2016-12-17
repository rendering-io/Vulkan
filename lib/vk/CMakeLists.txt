# Build the vk library.
add_library(vk buffer.c++
               device.c++
               instance.c++
               physical_device.c++
               pipeline.c++
               queue.c++
               shader_module.c++)
target_link_libraries(vk PUBLIC vulkan)

