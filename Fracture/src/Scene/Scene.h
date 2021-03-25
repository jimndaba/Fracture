#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <glm/glm.hpp>

#include "Component/ComponentManager.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Entity/EntityManager.h"
#include "Entity/Entity.h"
#include "AssetManager/AssetManager.h"
#include "Component/TransformComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/TagComponent.h"
#include "Component/RenderComponent.h"
#include "Component/LightComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Component/BoxColliderComponent.h"
#include "Component/ScriptComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "Logging/Logger.h"

namespace Fracture
{

	class Entity; 
	class TransformComponent;
	class InstancePool;
	class UUID;

	class Scene
	{

	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		std::shared_ptr<Entity> Root();


		virtual void onLoad() {};

		void addEntity(const std::shared_ptr<Entity>& entity);		
		void SetRoot(const std::shared_ptr<Entity>& entity);
		void Destroy(const std::shared_ptr<Entity>& entity);
		void Destroy(UUID id);
		std::shared_ptr<Entity> Duplicate(const std::shared_ptr<Entity>& entity);
		void clearScene();		

		static std::shared_ptr<Entity> ActiveCamera();
		static void setCamera(std::shared_ptr<Entity> camera);
		static void setCamera(UUID id);

		std::vector<std::shared_ptr<Entity>> Entities();
		static std::shared_ptr<Entity> GetEntity(UUID id);
		std::string Name;

	private:
		std::shared_ptr<Entity> m_root;
		static std::shared_ptr<Entity> active_Camera;
		static std::vector<std::shared_ptr<Entity>> m_entities;

		template <class T>
		void CopyComponentIfExists(const std::shared_ptr<Entity>& copyTo,const std::shared_ptr<Entity>& copyFrom);
		
	};

	template<class name>
	void Fracture::Scene::CopyComponentIfExists(const std::shared_ptr<Entity>& copyTo, const std::shared_ptr<Entity>& copyFrom)
	{
		if (ComponentManager::HasComponent<name>(copyFrom->GetId()))
		{		
			auto& comp = ComponentManager::GetComponent<name>(copyFrom->GetId());
			std::shared_ptr<name> component = std::dynamic_pointer_cast<name>(comp->clone(copyTo->GetId()));
			ComponentManager::AddComponent<name>(component);
			FRACTURE_INFO("ADD COMPONENT TO ENTITY: {}", copyTo->GetId());
		}
	}


}

#endif