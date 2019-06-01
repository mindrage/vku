cmake_minimum_required(VERSION 3.8)

project("vku")

# Options for this project.
option(VKU_BUILD_TESTS "Build Tests with glfw/glm" ON)
option(VKU_BUILD_STATIC "Build Static library" ON)

set(CMAKE_CXX_STANDARD 17)
# Set all binaries to output in a general bin folder.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find vulkan.
find_package(Vulkan REQUIRED)

# Add dependencies
option(BUILD_SHARED_LIBS "" off)
option(BUILD_STATIC_LIBS "" off)
option(GLM_TEST_ENABLE "" off)
add_subdirectory(deps/glm)

# Use root as include dir for "vku"
include_directories(./)

# Manage tests
if(VKU_BUILD_TESTS)
    add_subdirectory(test)
endif()

# Add the files into a single cpp files for static compilation.
if(VKU_BUILD_STATIC)
    file(GLOB_RECURSE vku_source_files vku/*.cpp)
    add_library(vku STATIC ${vku_source_files})
    target_include_directories(vku PUBLIC ./)
    target_link_libraries(vku glm Vulkan::Vulkan)
endif()