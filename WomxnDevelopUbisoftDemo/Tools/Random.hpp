#pragma once

#include <cmath>

inline const int GetRandomIntBetween(int min, int max)
{
    return rand() % max + min;
}

inline const float GetRandomFloat()
{
    // Generate random between 0.0 and 1.0 inclusive
    return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

inline const float GetRandomFloatBetween(float min, float max)
{
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}
