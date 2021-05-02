#include "stdafx.h"

#include "AnimationSystem.hpp"

#include "Components/Sprite.hpp"
#include "Core/Entity.hpp";
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <set>

void AnimationSystem::Update(World& world)
{
    std::set<Entity> entities = world.Find(GetSignature());

    for (auto entity : entities) {
        Animation& animation = world.GetComponent<Animation>(entity);
        Sprite& sprite = world.GetComponent<Sprite>(entity);

        int nextKeyframe = NextKeyframe(animation);
        if (nextKeyframe < 0) {
            continue;
        }

        sf::Vector2i currentOffset = animation.keyframes[animation.currentKeyframe];
        sf::Vector2i nextOffset = animation.keyframes[nextKeyframe];
        sf::IntRect spriteRect(nextOffset.x, nextOffset.y, animation.textureSize.x, animation.textureSize.y);

        sprite.sprite->setTextureRect(spriteRect);

        animation.currentKeyframe = nextKeyframe;
    }
}

int AnimationSystem::NextKeyframe(Animation& animation)
{
    switch (animation.loopMode) {
    case Loop:
        return NextKeyframeInLoopMode(animation);
    case LoopReverse:
        return NextKeyframeInLoopReverseMode(animation);
    default:
        break;
    }

    return -1;
}

int AnimationSystem::NextKeyframeInLoopReverseMode(Animation& animation)
{
    // Reverse when at end
    if (animation.currentKeyframe == animation.keyframes.size() - 1 && animation.increase) {
        animation.increase = false;
        return animation.currentKeyframe - 1;
    }
    // Reverse when at beginning
    if (animation.currentKeyframe == 0 && !animation.increase) {
        animation.increase = true;
        return animation.currentKeyframe + 1;
    }
    // Continue increasing
    if (animation.increase) {
        return animation.currentKeyframe + 1;
    }
    // Continue decreasing
    if (!animation.increase) {
        return animation.currentKeyframe - 1;
    }

    return -1;
}

int AnimationSystem::NextKeyframeInLoopMode(Animation& animation)
{
    if (animation.increase) {
        return (animation.currentKeyframe + 1) % animation.keyframes.size();
    } else if (!animation.increase) {
        return (animation.currentKeyframe - 1) % animation.keyframes.size();
    }

    return -1;
}
