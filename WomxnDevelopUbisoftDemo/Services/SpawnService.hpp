#pragma once

#include "Core/World.hpp"

class SpawnService {
public:
    void SpawnPlayer(World& world);
    void SpawnPlatform(World& world);
    void SpawnGround(World& world);
    void SpawnEdge(World& world);
    void SpawnBackground(World& world);

private:
    int GetRandomBetween(int windowXMin, int windowXMax);
};
