#pragma once

#include "../../common.hpp"
#include "queue.hpp"


namespace vku
{
    class logical_device
    {
    public:
        VkDevice device;
        VkQueue queue;

        ~logical_device();

        // TODO: remove move/copy operator

    };


}