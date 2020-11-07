#pragma once
#include <iostream>
#include "Editor/Editor.h"
#include "Editor/FractureSplash.h"
#include "Fracture.h"

int main(int argc, char* args[])
{
	std::unique_ptr<Fracture::Editor> FractureEngine = std::make_unique<Fracture::Editor>();
	Fracture::FractureSplash splash(FractureEngine.get());	
	if (!splash.Show())
	{
		return 0;
	}
	FractureEngine->run();
	return 0;
}