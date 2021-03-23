#include "stdafx.h"
#include "App.hpp"
#include "Collideable.hpp"
#include "Transformable.hpp"
#include "Renderable.hpp"
#include "RigidBody.hpp"
#include "CameraCenter.hpp"

static constexpr float APP_MAX_FRAMERATE{ 60.0f };
static const sf::Vector2u APP_WINDOW_SIZE{ 1024, 1000 };

App::App(const char* appName) : window{sf::VideoMode(APP_WINDOW_SIZE.x, APP_WINDOW_SIZE.y), appName, sf::Style::Titlebar | sf::Style::Close}
{
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(static_cast<uint32_t>(APP_MAX_FRAMERATE));
	window.setActive();
}

App& App::Build()
{
	RegisterComponents();
	RegisterSystems();

	return *this;
}

void App::Run()
{
	// Call initalization system
	spawnSystem->Spawn(&world);

	float deltaTime{ 1.0f / APP_MAX_FRAMERATE };
	sf::Clock clock;

	while (window.isOpen()) {

		clock.restart();

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		playerControlSystem->Update(&world, deltaTime);
		physicSystem->Update(&world, deltaTime);
		collisionSystem->Update(&world);

		renderSystem->Render(&world, &window);

		deltaTime = clock.getElapsedTime().asSeconds();
	}
}

void App::RegisterComponents()
{
	world.RegisterComponent<Transformable>();
	world.RegisterComponent<Renderable>();
	world.RegisterComponent<RigidBody>();
	world.RegisterComponent<CameraCenter>();
	world.RegisterComponent<Collideable>();
}

void App::RegisterSystems()
{
	spawnSystem = world.RegisterSystem<SpawnSystem>();
	Signature spawnSystemSignature{};
	world.SetSystemSignature<SpawnSystem>(spawnSystemSignature);

	renderSystem = world.RegisterSystem<RenderSystem>();
	Signature renderSystemSignature{};
	renderSystemSignature.set(world.GetComponent<Renderable>());

	world.SetSystemSignature<RenderSystem>(renderSystemSignature);
	playerControlSystem = world.RegisterSystem<PlayerControlSystem>();
	Signature playerControlSystemSignature{};
	playerControlSystemSignature.set(world.GetComponent<Transformable>());
	playerControlSystemSignature.set(world.GetComponent<Renderable>()); // find a way to remove it
	playerControlSystemSignature.set(world.GetComponent<RigidBody>());
	world.SetSystemSignature<PlayerControlSystem>(playerControlSystemSignature);

	physicSystem = world.RegisterSystem<PhysicSystem>();
	physicSystem->SetGravity(sf::Vector2f({ 0.0f, 100.0f }));
	Signature physicSystemSignature{};
	physicSystemSignature.set(world.GetComponent<Transformable>());
	physicSystemSignature.set(world.GetComponent<Renderable>()); // find a way to remove it 
	physicSystemSignature.set(world.GetComponent<RigidBody>());
	world.SetSystemSignature<PhysicSystem>(physicSystemSignature);

	collisionSystem = world.RegisterSystem<CollisionSystem>();
	Signature collisionSystemSignature{};
	collisionSystemSignature.set(world.GetComponent<Collideable>());
	collisionSystemSignature.set(world.GetComponent<RigidBody>());
	world.SetSystemSignature<CollisionSystem>(collisionSystemSignature);
}