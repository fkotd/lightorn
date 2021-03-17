#include "stdafx.h"
#include "PhysicSystem.hpp"
#include "Transformable.hpp"
#include "RigidBody.hpp"
#include "Renderable.hpp"

void PhysicSystem::SetGravity(sf::Vector2f gravity)
{
	this->gravity = gravity;
}

void PhysicSystem::Update(World* world, float deltaTime)
{
	std::set<Entity> entities = world->Find(GetSignature());

	for (auto entity : entities) {
		Transformable* transformable = world->GetComponent<Transformable>(entity);
		RigidBody* rigidBody = world->GetComponent<RigidBody>(entity);
		Renderable* renderable = world->GetComponent<Renderable>(entity);

		transformable->transformable.move(rigidBody->velocity * deltaTime);
		rigidBody->velocity += gravity * deltaTime;
		//renderable->shape->setPosition(transformable->transformable.getPosition());
	}
}