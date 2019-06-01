#pragma once

#include "../../common.hpp"

namespace vku
{
    class instance;

    class config
    {
    public:
        config() { }
        virtual ~config() { }
        virtual bool configure(instance& instance, VkInstanceCreateInfo& create_info, VkAllocationCallbacks& allocators) { return true; };
        virtual bool setup_instance(vku::instance& instance, VkInstance& instance_handle) { return true; }
        virtual bool destroy_instance(vku::instance& instance, VkInstance& instance_handle) { return true;  }
    };

}