#pragma once

#include "Core/World.hpp"

#include "Components/Animation.hpp"

class AnimationSystem : public System {
public:
    void Update(World& world);

private:
    int NextKeyframe(Animation& animation);
    int NextKeyframeInLoopReverseMode(Animation& animation);
};
