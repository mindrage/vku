#pragma once

#include "../../common.hpp"

namespace vku
{

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