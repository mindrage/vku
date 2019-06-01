#include "instance.hpp"

template<typename T>
vku::instance&
vku::instance::configure()
{
    configs.emplace_back(new T());
    return *this;
}

template<typename T, typename... Args>
vku::instance&
vku::instance::configure(Args&& ... args) {
    configs.emplace_back(new T(args...));
    return *this;
}