#pragma once
#ifndef FILEDIAGS_H
#define FILEDIAGS_H


#include <string>
#include "AssetManager/AssetManager.h"

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32


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