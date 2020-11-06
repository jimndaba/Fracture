#pragma once
#ifndef FILEDIAGS_H
#define FILEDIAGS_H


#include <string>
#include "SDL2/SDL.h"
#include <SDL2/SDL_syswm.h>

namespace Fracture
{

	class FileDialogue
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
		static std::string SelectDirectory(const char* filter);
	};

	

}

#endif