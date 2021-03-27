#pragma once

#include "Entity.hpp"
#include "Signature.hpp"
#include <set>

class System
{
public:
	void SetSignature(Signature signature)
	{
		this->signature = signature;
	}

	Signature GetSignature() const
	{
		return signature;
	}

private:
	Signature signature;
};
