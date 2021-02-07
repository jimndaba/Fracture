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

		TagComponent(const TagComponent& tag,const uint32_t& entityID) :Component(entityID, ComponentType::Tag)
		{
			Name = tag.Name;
			isVisible = tag.isVisible;
		}

		const TagComponent& operator = (const TagComponent& tag)
		{
			Name = tag.Name;
			isVisible = tag.isVisible;
			return *this;
		}

		virtual void onStart();

		std::string Name;
		bool isVisible = true;
		void SetName(const std::string& name);

		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<TagComponent> clone(uint32_t entityID) const
		{
			return std::shared_ptr<TagComponent>(this->clone_impl(entityID));
		}

	private:

		virtual TagComponent* clone_impl(uint32_t entityID) const override
		{
			return new TagComponent(*this,entityID);
		}

	};

}

#endif