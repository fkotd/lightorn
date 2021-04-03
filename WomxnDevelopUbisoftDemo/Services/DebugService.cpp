#include "stdafx.h"

#include "DebugService.hpp"

void DebugService::RenderDebugService(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu");
    ImGui::Text("Press F1 to close this debug menu");
    ImGui::NewLine();

    ImGui::Text("X: %f", 1);
    ImGui::Text("Y: %f", 2);

    ImGui::End();
}
