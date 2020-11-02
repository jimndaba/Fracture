#pragma once
#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include "Scene/Scene.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Component/Component.h"
#include "Component/ComponentManager.h"
#include "Component/TagComponent.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/ScriptComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/BoxColliderComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/LightComponent.h"
#include "AssetManager/EngineMaterials.h"
#include "Rendering/Material.h"
#include "Scene/Scene.h"

namespace Fracture
{

	class  EntityFactory
	{
	public:
		EntityFactory();

		static std::shared_ptr<Entity> CreateEmpty(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id,"Empty");
	
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			return newEntity;
		}

		static std::shared_ptr<Entity> CreateSunlight(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "sunLight");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);

			std::shared_ptr<LightComponent> light = std::make_shared<LightComponent>(newEntity->Id, LightType::Sun);
			ComponentManager::AddComponent(relationship);
			ComponentManager::AddComponent(light);

			return newEntity;
		}

		static std::shared_ptr<Entity> CreatePointlight(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Pointlight");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			std::shared_ptr<LightComponent> light = std::make_shared<LightComponent>(newEntity->Id, LightType::Point);
			ComponentManager::AddComponent(relationship);
			ComponentManager::AddComponent(light);
			return newEntity;
		}

		static std::shared_ptr<Entity> CreateSpotlight(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Spotlight");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			std::shared_ptr<LightComponent> light = std::make_shared<LightComponent>(newEntity->Id, LightType::Spot);
			ComponentManager::AddComponent(relationship);
			ComponentManager::AddComponent(light);

			return newEntity;
		}

		static std::shared_ptr<Entity> CreateCube(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Cube");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			std::string name = "CubeMat" + materialCount;
			std::shared_ptr<UnTexturedMaterial> material = std::make_shared<UnTexturedMaterial>(name,
				AssetManager::getShader("Primitives")
				);
			AssetManager::AddMaterial("cubeMat", material);
			ComponentManager::AddComponent<RenderComponent>(newEntity->Id,"Cube", name);
			materialCount++;
			return newEntity;
		}

		static std::shared_ptr<Entity> CreatePlane(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Plane");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			std::string name = "PlaneMat" + materialCount;
			std::shared_ptr<UnTexturedMaterial> material = std::make_shared<UnTexturedMaterial>(name,
				AssetManager::getShader("Primitives")
				);
			AssetManager::AddMaterial(name, material);
			ComponentManager::AddComponent<RenderComponent>(newEntity->Id, "Plane", name);
			materialCount++;
			return newEntity;
		}
		
		static std::shared_ptr<Entity> CreateSphere(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Sphere");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);

			std::string name = "SphereMat" + materialCount;
			std::shared_ptr<UnTexturedMaterial> material = std::make_shared<UnTexturedMaterial>(name,
				AssetManager::getShader("Primitives")
				);
			AssetManager::AddMaterial(name , material);
			ComponentManager::AddComponent<RenderComponent>(newEntity->Id, "Sphere", name);
			materialCount++;
			return newEntity;
		}
		
		static std::shared_ptr<Entity> CreateSuzane(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Cube");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			std::string name = "Suzane" + materialCount;
			std::shared_ptr<UnTexturedMaterial> material = std::make_shared<UnTexturedMaterial>(name,
				AssetManager::getShader("Primitives")
				);
			AssetManager::AddMaterial(name, material);
			ComponentManager::AddComponent<RenderComponent>(newEntity->Id, "Cube", name);
			materialCount++;
			return newEntity;
		}


	private:
		static int materialCount;
	};

}

#endif