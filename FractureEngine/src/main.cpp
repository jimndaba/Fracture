#pragma once
#include <iostream>
#include "Editor/Editor.h"
#include "Editor/FractureSplash.h"
#include "Fracture.h"

int main(int argc, char* args[])
{
	std::shared_ptr<Fracture::Editor> FractureEngine = Fracture::Editor::CreateInstance();
	std::unique_ptr<Fracture::FractureSplash> splash = std::make_unique<Fracture::FractureSplash>(FractureEngine.get());	
	
	if (!splash->Show())
	{
		return 0;
	}

	FractureEngine->run();
	FractureEngine.reset();

	return 0;
}