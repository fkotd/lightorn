#include "stdafx.h"

#include "Event.hpp"

Event::Event(std::any value, bool isPersistent)
    : value(value)
    , isPersistent(isPersistent)
{
}

std::any Event::GetValue() const
{
    return value;
}

bool Event::IsPersistent() const
{
    return isPersistent;
}
