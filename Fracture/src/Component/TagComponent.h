#pragma once
#ifndef TAGCOMPONENT_H
#define TAGCOMPONENT_H

#include "Component.h"

namespace Fracture
{

	class TagComponent:public Component
	{

	public:
		TagComponent(UUID entityID);
		TagComponent(UUID entityID,const std::string& name);
		~TagComponent() = default;

		TagComponent(const TagComponent& tag,const UUID& entityID) :Component(entityID)
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

		std::shared_ptr<TagComponent> clone(UUID id) const
		{
			return std::shared_ptr<TagComponent>(this->clone_impl(id));
		}

	private:

		virtual TagComponent* clone_impl(UUID id) const override
		{
			return new TagComponent(*this,id);
		}

	};

}

#endif