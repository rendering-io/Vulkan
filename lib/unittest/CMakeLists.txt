##
# lib/unittest/CMakeLists.txt
#

# Add a library that handles the actual running of tests.
add_library(unittest unittest.c++)
target_link_libraries(unittest PUBLIC GMock)

