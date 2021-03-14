#include <stdafx.h>

#include "App.hpp"

int main()
{
	// GameDemo game{};

	// game.RunGameLoop();

	App app{"Lightorn"};

	app.Build().Run();

	return EXIT_SUCCESS;
}