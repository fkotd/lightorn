#pragma once

#include <any>
#include <string>

class Event {
public:
    Event(std::any value, bool isPersistent);
    std::any GetValue() const; // TODO: passer le type en param de template
    bool IsPersistent() const;

private:
    std::any value;
    bool isPersistent;
};
