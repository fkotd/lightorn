#pragma once

#include "Entity.hpp"
#include <iostream>

class EntityManager {
public:
    Entity AddEntity()
    {
        return entityIdsCounter++;
    }

private:
    Entity entityIdsCounter = 0;
};
