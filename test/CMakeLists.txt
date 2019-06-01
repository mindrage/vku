cmake_minimum_required(VERSION 3.8)

# Add dependancies.
option(GLFW_BUILD_EXAMPLES "" off)
option(GLFW_BUILD_TESTS "" off)
option(GLFW_BUILD_DOCS "" off)
add_subdirectory(deps/glfw)

# Iterate all cpp files and add a target per file

file(GLOB VKU_TEST_FILES *.cpp)
foreach(file ${VKU_TEST_FILES})
    get_filename_component(file_clipped ${file} NAME_WE)
    string(JOIN "" target_name "vku_" ${file_clipped})
    message(${target_name})
    add_executable(${target_name} ${file})
    target_link_libraries(${target_name} glfw vku glm Vulkan::Vulkan)
endforeach()