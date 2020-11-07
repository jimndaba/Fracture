#pragma once
#ifndef FILEDIAGS_H
#define FILEDIAGS_H


#include <string>
#include "SDL2/SDL.h"
#include <SDL2/SDL_syswm.h>
#include "AssetManager/AssetManager.h"


namespace Fracture
{

	class FileDialogue
	{
	public:

		static std::string OpenFile(const char* filter);
		static std::string OpenFile(const char* filter,std::string& filename);
		static std::string SaveFile(const char* filter);
		static std::string AddAsset();
		static std::string SelectDirectory();
	};

	

}

#endif