#pragma once

#include "Tools/LoopMode.hpp"
#include <map>

struct Animation {
    std::map<int, sf::Vector2i> keyframes;
    LoopMode loopMode;
    int currentKeyframe;
    bool increase;
    sf::Vector2i textureSize;
};
