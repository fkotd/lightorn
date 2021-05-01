#include "stdafx.h"

#include "Event.hpp"

Event::Event(std::any value, bool isPersistent)
    : value(value)
    , isPersistent(isPersistent)
{
}

bool Event::IsPersistent() const
{
    return isPersistent;
}
