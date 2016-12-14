# Build the vk library.
add_library(vk device.c++
               instance.c++
	       physical_device.c++)
target_link_libraries(vk PUBLIC vulkan)

