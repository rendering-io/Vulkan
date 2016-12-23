##
# test/vk/CMakeLists.txt
#

# Add a unit test executable for testing the vk library.
add_executable(test-vk instance_tests.c++)
target_link_libraries(test-vk PUBLIC vk unittest)

# Register the test executable with ctest.
add_test(NAME vk COMMAND test-vk)
