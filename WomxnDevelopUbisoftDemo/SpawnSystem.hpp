#pragma once

#include "World.hpp"

class SpawnSystem : public System
{
public:
	void Spawn(World* world);

private:
	// FIXME : find another way to do that
	void SpawnPlayer(World* world);
	void SpawnElement(World* world);
};

