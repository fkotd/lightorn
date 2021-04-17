#pragma once

#include "Entity.hpp"
#include <functional>

struct ComponentInfo {
    std::function<void(Entity)> RemoveEntityCallBack;
};
