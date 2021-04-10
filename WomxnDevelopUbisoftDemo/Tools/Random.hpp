#pragma once

#include <cmath>

inline const int GetRandomBetween(int min, int max)
{
    return rand() % max + min;
}
