#pragma once

#include "Core/World.hpp"

class CommitSystem : public System {
public:
    void Commit(World& world);
};
