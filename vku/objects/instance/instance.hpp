#pragma once

#include "../config/config.hpp"
#include "../device/physical_device.hpp"
#include "../device/logical_device.hpp"

namespace vku
{

class instance
{
public:
    instance();

    ~instance();


    bool setup();

    void destroy();


    bool add_layer(const char* layer);

    bool layer_added(const char* layer);
    
    bool layer_supported(const char* layer);


    bool add_extension(const char* ext);

    bool extension_added(const char* ext1);

    bool extension_supported(const char* ext);

    std::unique_ptr<logical_device> create_device(device_selector* selector = nullptr);

    template<typename T>
    instance& configure();

    template<typename T, typename... Args>
    instance& configure(Args&& ... args);

    VkAllocationCallbacks* get_allocator_callbacks();
private:

    bool read_devices();

    std::vector<const char*> layers;
    std::vector<VkLayerProperties> available_layers;
    std::vector<const char*> extensions;
    std::vector<VkExtensionProperties> available_extensions;
    std::vector<std::unique_ptr<config>> configs;
    std::vector<physical_device> physical_devices;

    VkInstance instance_handle;
    VkInstanceCreateInfo create_info;
    VkAllocationCallbacks allocator_callbacks;
};

}