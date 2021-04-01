#pragma once

#include "Core/World.hpp"

class SpawnService {
public:
    void Spawn(const std::unique_ptr<World>& world);

private:
    // FIXME : maybe find another way to do that
    void SpawnPlayer(const std::unique_ptr<World>& world);
    void SpawnElement(const std::unique_ptr<World>& world);
    void SpawnBackground(const std::unique_ptr<World>& world, int numberOfElements);
};
