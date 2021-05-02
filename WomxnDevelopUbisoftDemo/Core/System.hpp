#pragma once

#include "Signature.hpp"

class System {
public:
    void SetSignature(Signature signature)
    {
        this->signature = signature;
    }

    Signature GetSignature() const
    {
        return signature;
    }

    virtual ~System() = default;

private:
    Signature signature;
};
