#pragma once

#include "config.hpp"

namespace vku
{

class debug_config : public config {
public:
    debug_config(bool enable_validation = true, PFN_vkDebugUtilsMessengerCallbackEXT debug_callback = nullptr);

    ~debug_config() {}

    bool configure(vku::instance& instance, VkInstanceCreateInfo& create_info, VkAllocationCallbacks& allocators) final override;
    bool setup_instance(vku::instance& instance, VkInstance& instance_handle) final override;
    bool destroy_instance(vku::instance& instance, VkInstance& instance_handle) final override;
private:
    void setup_debug_info(VkDebugUtilsMessengerCreateInfoEXT& debug_create_info);

    bool enable_validation;
    VkDebugUtilsMessengerCreateInfoEXT instance_debug_create_info;
    PFN_vkDebugUtilsMessengerCallbackEXT debug_callback;
    VkDebugUtilsMessengerEXT debug_messenger;
};


}