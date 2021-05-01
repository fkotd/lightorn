#pragma once

#include <any>

class Event {
public:
    Event(std::any value, bool isPersistent);
    bool IsPersistent() const;

    template <typename T>
    T GetValue() const
    {
        return std::any_cast<T>(value);
    }

private:
    std::any value;
    bool isPersistent;
};
