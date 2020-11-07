#pragma once
#ifndef PROJECTPROPERTIES_H
#define PROJECTPROPERTIES_H

#include "Fracture.h"
#include <string>

namespace Fracture
{

	struct ProjectProperties
	{

		std::string ProjectName;
		std::string ProjectDirectory;
		std::string ProjectFilePath;
		std::string GameConfigPath;

		//Content
		std::string ContentDirectory;
		std::string ScenesPath;
		std::string ShadersPath;
		std::string ModelsPath;
		std::string TexturesPath;
		std::string FontsPath;

		//Editor State
		std::string ActiveScene;

	};

}

#endif