#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include "Rendering/ISceneProbe.h"

namespace Fracture
{
	enum class ComponentType
	{
		None,
		Relationship,
		Tag,
		Transform,
		EditorNode,
		Render,
		Camera,		
		Rigidbody,
		BoxCollider,
		Script,
		Light,		
		Audio,
	};

	class Component
	{
	public:
		Component(uint32_t id,ComponentType mtype):EntityID(id),componentType(mtype)
		{
		};
		~Component() {};

		virtual void onStart() = 0;

		virtual void OnDebug() {};
		virtual void Accept(ISceneProbe* visitor) = 0;		
		uint32_t EntityID;
		ComponentType componentType = ComponentType::None;		
	};
	
}
#endif