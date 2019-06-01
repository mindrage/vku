#define VKU_EXAMPLE_NAME "01 - Create Instance"
#define VKU_EXAMPLE_WIDTH 1280
#define VKU_EXAMPLE_HEIGHT 720

//
#include <vku/vku.hpp>

// 
#include <GLFW/glfw3.h>
#include <vku/objects/config/glfw_config.hpp>

#include <iostream>


int main(int argc, char** argv)
{
    std::cout << "Starting example: " << VKU_EXAMPLE_NAME << std::endl;

    // Initialize the windows.
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(VKU_EXAMPLE_WIDTH, VKU_EXAMPLE_HEIGHT, VKU_EXAMPLE_NAME, nullptr, nullptr);
    
    
    vku::instance vk;
    vk.configure<vku::glfw_config>();
    vk.configure<vku::app_config>();
    vk.configure<vku::debug_config>();
    
    if (vk.setup())
    {

        // Update window
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }

    vk.destroy();
    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}