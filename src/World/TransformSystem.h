#pragma once
#ifndef TRANSFORMSYSTEM_H
#define TRANSFORMSYSTEM_H

#include "SceneManager.h"

namespace Fracture
{
	struct TransformComponent;
	struct TransformSystem
	{
		TransformSystem();

		static void Update(Fracture::SceneManager* manager, Fracture::UUID root);
		static void UpdatePrefabs(Fracture::SceneManager* manager, Fracture::UUID root);
		static void UpdatePrefab(Fracture::SceneManager* manager, Fracture::UUID root);

		static void Translate(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value);		
		static void LookAt(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value, const glm::vec3& up);
		static void SetPosition(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value);
		static void SetScale(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value);
		static void SetRotation(const std::shared_ptr<TransformComponent>& transform, const glm::vec3& value);
	};



}


#endif