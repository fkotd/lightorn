#pragma once

#include "Entity.hpp"
#include <unordered_map>

class EventManager {
public:
    void AddGameEvent(std::string name, std::any value)
    {
        gameEvents.emplace(name, value);
    }

    std::any GetGameEvent(std::string name)
    {
        auto it = gameEvents.find(name);
        if (it == gameEvents.end()) {
            return nullptr;
        }

        return it->second;
    }

    void ClearEvents()
    {
        gameEvents.clear();
    }

private:
    std::unordered_map<std::string, std::any> gameEvents;
};
