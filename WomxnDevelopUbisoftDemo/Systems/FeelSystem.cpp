#include "stdafx.h"

#include "FeelSystem.hpp"

#include "Components/CameraCenter.hpp"
#include "Components/Feel.hpp"
#include "Components/Gripper.hpp"
#include "Components/Sprite.hpp"
#include "Tools/Messages.hpp"

void FeelSystem::Update(World& world, float deltaTime)
{
    std::set<Entity> entities = world.Find(GetSignature());

    Feeling grippedFeeling = GetFeelingChange(world);
    ImGui::Begin("Feel Sytem Infos");
    ImGui::Text("Event feeling: %d", grippedFeeling);
    ImGui::End();

    for (auto entity : entities) {
        Sprite& sprite = world.GetComponent<Sprite>(entity);
        Feel& feel = world.GetComponent<Feel>(entity);

        sf::Color spriteColor = sprite.sprite->getColor();
        if (feel.feeling != grippedFeeling && grippedFeeling != Feeling::Neutral) {
            sprite.sprite->setColor(sf::Color(spriteColor.r, spriteColor.g, spriteColor.b, 0));
        } else {
            sprite.sprite->setColor(sf::Color(spriteColor.r, spriteColor.g, spriteColor.b, 255));
        }
    }
}

Feeling FeelSystem::GetFeelingChange(World& world)
{
    Event* feelingChange = world.GetGameEvent(FEELING_CHANGE);
    Feeling feeling = Feeling::Neutral;

    if (feelingChange != nullptr) {
        try {
            feeling = std::any_cast<Feeling>(feelingChange->GetValue());
        } catch (const std::bad_any_cast& _) {
            std::cout << "feeling bad any cast" << std::endl;
        }
    }

    return feeling;
}
