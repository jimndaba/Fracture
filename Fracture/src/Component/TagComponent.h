#pragma once
#ifndef TAGCOMPONENT_H
#define TAGCOMPONENT_H

#include "Component.h"
#include <string>

namespace Fracture
{

	class TagComponent:public Component
	{

	public:
		TagComponent(int entityID);
		TagComponent(int entityID,std::string name);
		~TagComponent();

		virtual void onAttach();
		virtual void onDettach();

		int ID;
		std::string Name;

		void SetName(std::string name);
	};

}

#endif