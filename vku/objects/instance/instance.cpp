#pragma once

#include "instance.inl"

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

    return usable;
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

