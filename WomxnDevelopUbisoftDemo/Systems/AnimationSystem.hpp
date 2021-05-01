#pragma once

#include "Components/Animation.hpp"
#include "Core/World.hpp"

class AnimationSystem : public System {
public:
    void Update(World& world);

private:
    int NextKeyframe(Animation& animation);
    int NextKeyframeInLoopReverseMode(Animation& animation);
};
