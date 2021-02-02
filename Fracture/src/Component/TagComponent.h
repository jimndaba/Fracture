#pragma once
#ifndef TAGCOMPONENT_H
#define TAGCOMPONENT_H

#include "Component.h"

namespace Fracture
{

	class TagComponent:public Component
	{

	public:
		TagComponent(uint32_t entityID);
		TagComponent(uint32_t entityID,const std::string& name);
		~TagComponent() = default;

		TagComponent(const TagComponent& tag) :Component(EntityID, ComponentType::Tag)
		{
			Name = tag.Name + "Copy";
			isVisible = tag.isVisible;
		}

		const TagComponent& operator = (const TagComponent& tag)
		{
			Name = tag.Name + "Copy";
			isVisible = tag.isVisible;
			return *this;
		}


		virtual void onStart();

		std::string Name;
		bool isVisible = true;
		void SetName(const std::string& name);

		void Accept(ISceneProbe* visitor) override;
	};

}

#endif