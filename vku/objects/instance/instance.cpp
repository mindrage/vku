#pragma once

#include "instance.inl"
#include "../device/default_selector.hpp"
#include "../device/logical_device.hpp"

vku::instance::instance() :
    layers(),
    available_layers(),
    extensions(),
    available_extensions(),
    configs(),

    create_info { },
    instance_handle( nullptr ),
    allocator_callbacks{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }
{
    // Get all avaialible layers.
    uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    available_layers.resize(layer_count);
    vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

    uint32_t extension_count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
    available_extensions.resize(extension_count);
    vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, available_extensions.data());

}

vku::instance::~instance()
{
    destroy();
}

bool
vku::instance::setup()
{
    // Set the struct type to start with.

    bool usable = true;

    // Run all configurations in order.
    for(auto& conf: configs) {
        usable = usable && conf->configure(*this, create_info, allocator_callbacks);
    }

    if (!usable)
        return false;


    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.enabledExtensionCount = (uint32_t) extensions.size();
    create_info.ppEnabledExtensionNames = extensions.data();
    create_info.enabledLayerCount = (uint32_t) layers.size();
    create_info.ppEnabledLayerNames = layers.data();

    if (vkCreateInstance(&create_info, get_allocator_callbacks(), &instance_handle) != VK_SUCCESS)
    {
        vkDestroyInstance(instance_handle, get_allocator_callbacks());
        return false;
    }

    usable = true;
    for (auto& conf : configs)
    {
        usable = usable && conf->setup_instance(*this, instance_handle);
    }

    if (!usable)
    {
        return false;
    }

    read_devices();

    return true;
}

bool
vku::instance::read_devices()
{

    // Fetch all physical devices
    uint32_t physical_device_count = 0;
    vkEnumeratePhysicalDevices(instance_handle, &physical_device_count, nullptr);
    std::vector<VkPhysicalDevice> p_devices(physical_device_count);
    vkEnumeratePhysicalDevices(instance_handle, &physical_device_count, p_devices.data());

    // Loop through and place them interleaved with thier properties and features.
    for (int i = 0; i < p_devices.size(); i++)
    {
        auto& phys_device = physical_devices.emplace_back();
        phys_device.device = p_devices[i];
        vkGetPhysicalDeviceFeatures(phys_device.device, &phys_device.features);
        vkGetPhysicalDeviceProperties(phys_device.device, &phys_device.properties);
        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(phys_device.device, &queue_family_count, nullptr);
        phys_device.queue_families.resize(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(phys_device.device, &queue_family_count, phys_device.queue_families.data());
    }

    return true;
}


void
vku::instance::destroy()
{
    if (instance_handle != nullptr)
    {

        if (instance_handle != nullptr)
        {
            bool destroyed = true;
            for (auto& conf : configs)
            {
                destroyed = destroyed && conf->destroy_instance(*this, instance_handle);
            }

            vkDestroyInstance(instance_handle, get_allocator_callbacks());
            instance_handle = nullptr;
        }
    }
}

VkAllocationCallbacks*
vku::instance::get_allocator_callbacks()
{
    if (allocator_callbacks.pfnAllocation != nullptr
        || allocator_callbacks.pfnFree != nullptr
        || allocator_callbacks.pfnInternalAllocation != nullptr
        || allocator_callbacks.pfnInternalFree != nullptr
        || allocator_callbacks.pfnReallocation != nullptr
        || allocator_callbacks.pUserData != nullptr
        ) {
        return &allocator_callbacks;
    }

    return nullptr;
}


bool 
vku::instance::add_layer(const char* layer)
{
    if (layer_supported(layer)) {
        layers.push_back(layer);
        return true;
    }
    return false;
}

bool
vku::instance::layer_added(const char* layer)
{
    for (const auto& l : layers)
    {
        if (strcmp(layer, l) == 0)
            return true;
    }
    return false;
}

bool
vku::instance::layer_supported(const char* layer)
{
    for (const auto& l : available_layers)
    {
        if (strcmp(layer, l.layerName) == 0)
            return true;
    }

    return false;
}

bool
vku::instance::add_extension(const char* ext)
{
    if (extension_supported(ext)) {
        extensions.push_back(ext);
        return true;
    }
    return false;
}

bool
vku::instance::extension_added(const char* ext)
{
    for (const auto& ex : extensions)
    {
        if (strcmp(ext, ex) == 0)
            return true;
    }
    return false;
}

bool
vku::instance::extension_supported(const char* ext)
{
    for (const auto& ex : available_extensions)
    {
        if (strcmp(ext, ex.extensionName) == 0)
            return true;
    }

    return false;
}


std::unique_ptr<vku::logical_device>
vku::instance::create_device(device_selector* const selector)
{
    physical_device* phys_device = nullptr;
    if (selector == nullptr)
        phys_device = default_device_selector().select(physical_devices);
    else {
        phys_device = selector->select(physical_devices);
    }

    if (phys_device == nullptr)
    {
        // TODO: Add error message.
        return nullptr;
    }

    // TODO: Future design for multiple queues?
    VkDeviceQueueCreateInfo queue_create_info = {};

    float queue_priority = 1.0f;
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = phys_device->queue_index;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures device_features = {};

    VkDeviceCreateInfo device_create_info = {};
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pEnabledFeatures = &device_features;
    device_create_info.enabledExtensionCount = 0;
    device_create_info.enabledLayerCount = (uint32_t)layers.size();
    device_create_info.ppEnabledLayerNames = layers.data();

    std::unique_ptr<logical_device> log_device = std::make_unique<logical_device>();

    if (vkCreateDevice(phys_device->device, &device_create_info, get_allocator_callbacks(), &log_device->device) != VK_SUCCESS) {
        return nullptr;
    }

    vkGetDeviceQueue(log_device->device, phys_device->queue_index, 0, &log_device->queue);

    return std::move(log_device);

}
