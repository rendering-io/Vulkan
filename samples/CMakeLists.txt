# Build instance sample.
add_executable(vector_add vector_add.c++)
target_link_libraries(vector_add PRIVATE vk)

# Copy the shader to the output directory.
configure_file(vector_add.spv ${CMAKE_BINARY_DIR}/vector_add.spv COPYONLY)

# Build xcb sample.
add_executable(xcb-demo xcb.c++)
target_link_libraries(xcb-demo PRIVATE vk xcb)
