##
# test/vk/CMakeLists.txt
#

set(TEST_SOURCES device_fixture.c++
                 image_tests.c++
                 instance_tests.c++)

# Add a unit test executable for testing the vk library.
add_executable(test-vk ${TEST_SOURCES})
target_link_libraries(test-vk PUBLIC vk unittest)

# Register the test executable with ctest.
add_test(NAME vk COMMAND test-vk)
