#include "stdafx.h"

#include "FeelSystem.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Feel.hpp"
#include "Components/Gripper.hpp"
#include "Components/Sprite.hpp"
#include "Tools/Messages.hpp"

const uint8_t FADE_OUT = 4;
const uint8_t FADE_IN = 20;

void FeelSystem::Update(World& world)
{
    std::set<Entity> entities = world.Find(GetSignature());

    Feeling gripFeeling = GetFeelingChange(world);

    ImGui::Begin("Feel Sytem Infos");
    ImGui::Text("Event feeling: %d", gripFeeling);
    ImGui::End();

    for (auto entity : entities) {
        Sprite& sprite = world.GetComponent<Sprite>(entity);
        Feel& feel = world.GetComponent<Feel>(entity);

        sf::Color spriteColor = sprite.sprite->getColor();
        if (feel.feeling != gripFeeling && gripFeeling != Feeling::Neutral) {
            // fade out
            uint8_t a = std::max(0, spriteColor.a - FADE_OUT);
            sprite.sprite->setColor(sf::Color(spriteColor.r, spriteColor.g, spriteColor.b, a));
        } else {
            // fade in
            uint8_t a = std::min(255, spriteColor.a + FADE_IN);
            sprite.sprite->setColor(sf::Color(spriteColor.r, spriteColor.g, spriteColor.b, a));
        }
    }
}

Feeling FeelSystem::GetFeelingChange(World& world)
{
    Event* gripFeelingEvent = world.GetGameEvent(GRIP_FEELING);
    Feeling gripFeeling = Feeling::Neutral;

    if (gripFeelingEvent != nullptr) {
        try {
            gripFeeling = std::any_cast<Feeling>(gripFeelingEvent->GetValue());
        } catch (const std::bad_any_cast& _) {
        }
    }

    return gripFeeling;
}
