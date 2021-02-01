#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/quaternion.hpp>

#include "glm/glm.hpp"
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

		virtual ~Component() = 0;
		virtual void onStart() = 0;
		virtual void Accept(ISceneProbe* visitor) = 0;

		virtual void OnDebug() {};
	
		uint32_t EntityID;
		ComponentType componentType = ComponentType::None;		
	};
	
	
}
#endif