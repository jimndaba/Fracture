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
		TagComponent(uint32_t entityID);
		TagComponent(uint32_t entityID,std::string name);
		~TagComponent();


		virtual void onStart();

		int ID;
		std::string Name;
		bool isVisible = true;
		void SetName(std::string name);
	};

}

#endif