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
		TagComponent(uint32_t entityID,const std::string& name);
		~TagComponent() = default;

		virtual void onStart();

		std::string Name;
		bool isVisible = true;
		void SetName(const std::string& name);

		void Accept(ISceneProbe* visitor) override;
	};

}

#endif