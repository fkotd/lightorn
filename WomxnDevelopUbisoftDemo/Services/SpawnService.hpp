#pragma once

#include "Core/World.hpp"

class SpawnService {
public:
    void SpawnPlayer(World& world);
    void SpawnElement(World& world);
    void SpawnBackground(World& world);

private:
    int GetRandomBetween(int windowXMin, int windowXMax);
};
