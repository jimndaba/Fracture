#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstdint>

namespace Fracture
{
	class TransformComponent:public Component
	{
	public:
		TransformComponent(uint32_t entityID);
		TransformComponent(uint32_t entityID, glm::vec3 pos);
		TransformComponent(uint32_t entityID, glm::vec3 pos, glm::vec3 scale);
		TransformComponent(uint32_t entityID, glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);
		~TransformComponent();

		virtual void onStart();

		glm::vec3 Position;
		glm::vec3 Scale;
		glm::vec3 Rotation;

		glm::mat4 GetLocalTranform();
		glm::mat4 GetWorldTransform();

	private:
		glm::mat4 m_LocalTransform;
		glm::mat4 m_WorldTransform; 		
	};

}

#endif
