#pragma once
#include <iostream>
#include "Editor/Editor.h"
#include "Editor/FractureSplash.h"
#include "Fracture.h"

int main(int argc, char* args[])
{
	Fracture::Editor FractureEngine;
	Fracture::FractureSplash splash(FractureEngine);
	
	if (splash.Show())
	{
		FractureEngine.run();
	}
	return 0;
}