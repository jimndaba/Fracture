#pragma once
#include <iostream>
#include "Editor/Editor.h"
#include "Fracture.h"

int main(int argc, char* args[])
{

	Fracture::Editor FractureEngine;
	FractureEngine.run();

	return 0;
}