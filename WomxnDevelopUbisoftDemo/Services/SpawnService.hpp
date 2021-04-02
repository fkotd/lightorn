#pragma once

#include "Core/World.hpp"

class SpawnService {
public:
    void Spawn(World& world);

private:
    // FIXME : maybe find another way to do that
    void SpawnPlayer(World& world);
    void SpawnElement(World& world);
    void SpawnBackground(World& world, int numberOfElements);
};
