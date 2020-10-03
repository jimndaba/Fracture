#pragma once
#include <iostream>
#include "SDL2/SDL.h"
#include "Application.h"
#include "Fracture.h"

int main(int argc, char* args[])
{

	Fracture::Application app;
	app.run();

	return 0;
}