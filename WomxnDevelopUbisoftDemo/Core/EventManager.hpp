#pragma once

#include "Entity.hpp"
#include "Event.hpp"
#include <unordered_map>

class EventManager {
public:
    void AddGameEvent(const std::string& name, Event event)
    {
        gameEvents.emplace(name, event);
    }

    void RemoveGameEvent(const std::string& name)
    {
        gameEvents.erase(name);
    }

    Event* GetGameEvent(const std::string& name)
    {
        auto it = gameEvents.find(name);
        if (it == gameEvents.end()) {
            return nullptr;
        }

        return &it->second;
    }

    void ClearEvents()
    {
        decltype(gameEvents) newGameEvents;

        for (auto&& event : gameEvents) {
            if (event.second.IsPersistent()) {
                newGameEvents.emplace(std::move(event));
            }
        }

        gameEvents.swap(newGameEvents);
    }

    void ClearAllEvents()
    {
        gameEvents.clear();
    }

private:
    std::unordered_map<std::string, Event> gameEvents;
};
