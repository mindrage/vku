#pragma once

#include <vku/objects/config/config.hpp>


namespace vku
{

class glfw_config : public config
{
public:
    glfw_config() : vku::config()
    {
        

    }

    ~glfw_config() {
    }

    bool configure(vku::instance& instance, VkInstanceCreateInfo& create_info, VkAllocationCallbacks& allocators) final override
    {
        uint32_t count = 0;
        const char** extensions = nullptr;
        extensions = glfwGetRequiredInstanceExtensions(&count);
        for (int i = 0; i < count; i++)
        {
            instance.add_extension(extensions[i]);
        }

        return true;
    }
};

}