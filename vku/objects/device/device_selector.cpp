#include "default_selector.hpp"



vku::physical_device* const
vku::default_device_selector::select(std::vector<physical_device>& devices)
{
    // No devices found.. TODO: Add better error reporting.
    if (devices.size() == 0)
        return nullptr;

    for (auto& device : devices)
    {
        int i = 0;
        for (auto& queue_family : device.queue_families)
        {
            if (queue_family.queueCount > 0 && queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                device.queue_index = i;
                return &device;
            }

            i++;
        }
    }



    return nullptr;
}

