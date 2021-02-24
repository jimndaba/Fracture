#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

#include <cstdint>
#include <memory>
#include <vector>
#include <math.h>
#include <string>

#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/compatibility.hpp"
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>

#include "Logging/Logger.h"
#include "Rendering/ISceneProbe.h"
#include "Entity/UUID.h"

namespace Fracture
{
	class UUID;

	class Component
	{
	public:
		Component(UUID id):m_ID(id)
		{
		};

		virtual ~Component() {};
		virtual void onStart() = 0;
		virtual void Accept(ISceneProbe* visitor) = 0;
		virtual void Accept(ISceneProbe* visitor, float dt) {};


		std::shared_ptr<Component> clone(UUID id) const
		{
			return std::shared_ptr<Component>(this->clone_impl(id));
		};

		virtual void OnDebug() {};
	
		const UUID& GetID()const
		{
			return m_ID;
		};

	private:
		virtual Component* clone_impl(UUID id) const = 0;

		UUID m_ID;
	};
	
	
}
#endif