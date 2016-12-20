# Build instance sample.
add_executable(instance instance.c++)
target_link_libraries(instance PRIVATE vk)

# Copy the shader to the output directory.
configure_file(vector_add.spv ${CMAKE_BINARY_DIR}/vector_add.spv COPYONLY)

