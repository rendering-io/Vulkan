# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.5)

# Declare some options for our build.
option(ENABLE_VALIDATION "Enable Vulkan validation layers." ON)
option(BUILD_UNITTESTS "Enable unit tests." ON)

# We should generate a compilation database to help YouCompleteMe with autocompletion.
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Globally add the include directory to all targets.
include_directories(include)

# Globally set compile options.
add_compile_options(-std=c++14 -Wall -Wpedantic -Werror)
if (${ENABLE_VALIDATION})
  add_definitions(-DENABLE_VALIDATION)
endif()

# Build the ext subdirectory.
add_subdirectory(ext)

# Build the lib subdirectory.
add_subdirectory(lib)

# Build unit tests.
if(${BUILD_UNITTESTS})
  add_subdirectory(test)
endif()

# Build samples.
add_subdirectory(samples)

