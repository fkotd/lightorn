#pragma once

#include <any>
#include <string>

class Event {
public:
    Event(std::any value, bool isPersistent);
    std::any GetValue() const;
    bool IsPersistent() const;

private:
    std::any value;
    bool isPersistent;
};
