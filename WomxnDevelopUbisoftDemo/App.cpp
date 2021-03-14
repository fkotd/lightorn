#include "stdafx.h"
#include "App.hpp"
#include "Position.hpp"
#include "Renderable.hpp"

static constexpr float APP_MAX_FRAMERATE{ 60.0f };
static const sf::Vector2u APP_WINDOW_SIZE{ 1024, 768 };

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
	// Call initalization systems
	spawnSystem->Spawn(&world);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		playerControlSystem->Update(&world);
		renderSystem->Render(&world, &window);
	}
}

void App::RegisterComponents()
{
	world.RegisterComponent<Position>();
	world.RegisterComponent<Renderable>();
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
	playerControlSystemSignature.set(world.GetComponent<Position>());
	playerControlSystemSignature.set(world.GetComponent<Renderable>());
	world.SetSystemSignature<PlayerControlSystem>(playerControlSystemSignature);
}