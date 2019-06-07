#pragma once

#include "../../common.hpp"

namespace vku
{

    class logical_device
    {
    public:
        VkDevice device;

        ~logical_device() {
            if (device != nullptr) vkDestroyDevice(device, nullptr);
        }

        // TODO: remove move/copy operator
    };


    class physical_device
    {
    public:
        VkPhysicalDevice device;
        VkPhysicalDeviceProperties properties;
        VkPhysicalDeviceFeatures features;
        std::vector<VkQueueFamilyProperties> queue_families;
        uint32_t queue_index = 0;

    };


    class device_selector
    {
    public:
        virtual ~device_selector() = default;

        virtual vku::physical_device* const
        select(std::vector<physical_device>& devices) = 0;
    };

}