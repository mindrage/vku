#include "logical_device.hpp"


vku::logical_device::~logical_device()
{
   if (device != nullptr) vkDestroyDevice(device, nullptr);
}
