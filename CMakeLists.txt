# Set the minimum required CMake version.
cmake_minimum_required(VERSION 3.5)

# We should generate a compilation database to help YouCompleteMe with autocompletion.
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

# Globally add the include directory to all targets.
include_directories(include)

# Globally set compile options.
add_compile_options(-std=c++14)

# Build the lib subdirectory.
add_subdirectory(lib)

# Build samples.
add_subdirectory(samples)

