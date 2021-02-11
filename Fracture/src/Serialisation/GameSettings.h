#pragma once 
#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <string>

namespace Fracture
{

	struct GameSettings
	{
		std::string Title = "Sandbox";
		//Display Settings
		int Resolution_Width = 1280;
		int Resolution_Height = 720;
		bool IsFullScreen;
		bool IsResizable;
		//FrameGraph Settings

		//Game Setup

		//System Settings


	};


}

#endif