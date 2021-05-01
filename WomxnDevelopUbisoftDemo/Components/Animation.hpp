#pragma once

#include "Tools/LoopMode.hpp"
#include <SFML/System/Vector2.hpp>
#include <map>

struct Animation {
    std::map<int, sf::Vector2i> keyframes;
    LoopMode loopMode;
    int currentKeyframe;
    bool increase;
    sf::Vector2i textureSize;
};
