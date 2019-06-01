#pragma once

#include "app.hpp"

vku::app_config::app_config(version_info api, named_info app, named_info engine) :
    config(),
    app_info({}),
    api_version(api),
    app(app),
    engine(engine)
{
}

vku::app_config::~app_config()
{

}

bool
vku::app_config::configure(vku::instance& instance, VkInstanceCreateInfo& create_info, VkAllocationCallbacks& allocators)
{
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.apiVersion = VK_MAKE_VERSION(api_version.major, api_version.minor, api_version.patch);
    app_info.pApplicationName = app.name;
    app_info.applicationVersion = VK_MAKE_VERSION(app.version.major, app.version.minor, app.version.patch);
    app_info.pEngineName = engine.name;
    app_info.engineVersion = VK_MAKE_VERSION(engine.version.major, engine.version.minor, engine.version.patch);

    create_info.pApplicationInfo = &app_info;
    return true;
}


