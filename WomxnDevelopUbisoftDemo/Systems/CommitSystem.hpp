#pragma once

#include "Core/System.hpp"
#include "Core/World.hpp"

class CommitSystem : public System {
public:
    void Commit(World& world);
};
