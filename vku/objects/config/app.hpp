#pragma once

#include "config.hpp"

namespace vku
{

    struct version_info {
        unsigned major = 1, minor = 0, patch = 0;
    };

    struct named_info {
        const char* name;
        version_info version;
    };

    class app_config : public config {
    public:

        app_config(version_info api = {}, named_info app = { "VKU Application", {} }, named_info engine = { "VKU Engine", {} }); 

        ~app_config();

        bool configure(vku::instance& instance, VkInstanceCreateInfo& create_info, VkAllocationCallbacks& allocator) final override;

    private:
        VkApplicationInfo app_info;
        version_info api_version;
        named_info app, engine;
    };


}