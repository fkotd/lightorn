#include "stdafx.h"

#include "DebugService.hpp"

void DebugService::RenderDebugService(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu");
    ImGui::Text("Press F1 to close this debug menu");
    ImGui::NewLine();

    ImGui::Text("Hello");

    ImGui::End();
}
