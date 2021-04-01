#pragma once

#include "Entity.hpp"
#include <iostream>
#include <queue>

class EntityManager {
public:
    Entity AddEntity()
    {
        return entityIdsCounter++;
    }

private:
    Entity entityIdsCounter = 0;
};
