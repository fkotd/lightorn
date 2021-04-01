#include <stdafx.h>

#include "App.hpp"

int main()
{
    App app { "Lightorn" };

    app.Build().Run();

    return EXIT_SUCCESS;
}