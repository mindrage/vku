#include "debug.hpp"

#include "../instance/instance.inl"

#include <iostream>

static VKAPI_ATTR
VkBool32 VKAPI_CALL
default_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData
)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}


VkResult vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}


vku::debug_config::debug_config(bool enable_validation, PFN_vkDebugUtilsMessengerCallbackEXT debug_callback) :
    config(),
    debug_callback(debug_callback),
    enable_validation(enable_validation)
{
    instance_debug_create_info = {};
}



bool
vku::debug_config::configure(vku::instance& instance, VkInstanceCreateInfo& create_info, VkAllocationCallbacks& allocators)
{

    if (enable_validation && instance.add_layer("VK_LAYER_KHRONOS_validation"))
    {
        setup_debug_info(instance_debug_create_info);
        instance.add_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        create_info.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &instance_debug_create_info;
    }
    else
    {
        create_info.pNext = nullptr;
        enable_validation = false;
    }

    return true;
}

bool vku::debug_config::setup_instance(vku::instance& instance, VkInstance& instance_handle)
{
    if (!enable_validation)
    {
        return true;
    }

    VkDebugUtilsMessengerCreateInfoEXT debugger_debug_create_info;
    setup_debug_info(debugger_debug_create_info);

    VkResult res = vkCreateDebugUtilsMessengerEXT(
        instance_handle,
        &debugger_debug_create_info,
        instance.get_allocator_callbacks(),
        &debug_messenger
    );

    if (res != VK_SUCCESS)
    {
        return false;
    }


    return true;
}


bool vku::debug_config::destroy_instance(vku::instance& instance, VkInstance& instance_handle)
{
    if (!enable_validation)
    {
        return true;
    }

    vkDestroyDebugUtilsMessengerEXT(instance_handle, debug_messenger, instance.get_allocator_callbacks());

    return true;
}



void
vku::debug_config::setup_debug_info(VkDebugUtilsMessengerCreateInfoEXT& debug_create_info)
{
    debug_create_info = {};
    debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    debug_create_info.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    debug_create_info.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
    debug_create_info.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    debug_create_info.messageType |= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
    debug_create_info.messageType |= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debug_create_info.messageType |= VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    debug_create_info.pfnUserCallback = debug_callback == nullptr ? (&default_debug_callback) : debug_callback;
}


