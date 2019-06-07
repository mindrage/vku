#pragma once

#include "device.hpp"

namespace vku
{

    class default_device_selector : public device_selector
    {
    public:

        vku::physical_device* const
        select(std::vector<physical_device>& devices) final override;
    };

}