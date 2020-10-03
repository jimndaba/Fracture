#pragma once
#include <iostream>
#include "Application.h"
#include "Fracture.h"

int main(int argc, char* args[])
{

	Fracture::Application FractureEngine;
	FractureEngine.run();

	return 0;
}