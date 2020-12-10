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
#include "Component/EditorNodeComponent.h"
#include "AssetManager/AssetManager.h"
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
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
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

			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<LightComponent>(light);

			std::shared_ptr<EditorNode> node = std::make_shared<EditorNode
			>(newEntity->Id);

			node->SetRotation(light->GetDirection());
			ComponentManager::AddComponent<EditorNode>(node);
			

			return newEntity;
		}

		static std::shared_ptr<Entity> CreatePointlight(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Pointlight");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			std::shared_ptr<LightComponent> light = std::make_shared<LightComponent>(newEntity->Id, LightType::Point);	
			
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<LightComponent>(light);
			ComponentManager::AddComponent<EditorNode>(newEntity->Id);
			return newEntity;
		}

		static std::shared_ptr<Entity> CreateSpotlight(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Spotlight");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			std::shared_ptr<LightComponent> light = std::make_shared<LightComponent>(newEntity->Id, LightType::Spot);
			
		
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<LightComponent>(light);

			std::shared_ptr<EditorNode> node = std::make_shared<EditorNode
			>(newEntity->Id);

			node->SetPosition(light->GetPosition());
			node->SetRotation(light->GetDirection());
			ComponentManager::AddComponent<EditorNode>(node);

			return newEntity;
		}

		static std::shared_ptr<Entity> CreateSkylight(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Skylight");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			std::shared_ptr<LightComponent> light = std::make_shared<LightComponent>(newEntity->Id, LightType::Sky);


			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<LightComponent>(light);

			return newEntity;
		}

		static std::shared_ptr<Entity> CreateCube(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Cube");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			std::string name = "CubeMat" + std::to_string(materialCount);
			std::shared_ptr<Material> material = std::make_shared<Material>(name,
				AssetManager::getShader("PrimitiveMaterial"));

			material->setColor3("material.diffuse", glm::vec3(1.0f));
			material->setColor3("material.ambient", glm::vec3(1.0f));
			material->setColor3("material.specular", glm::vec3(1.0f));
			material->setFloat("material.shininess", 64.0f);

			AssetManager::AddMaterial(name, material);
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
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			std::string name = "PlaneMat" + std::to_string(materialCount);
			std::shared_ptr<Material> material = std::make_shared<Material>(name,
				AssetManager::getShader("PrimitiveMaterial"));

			material->setColor3("material.diffuse", glm::vec3(1.0f));
			material->setColor3("material.ambient", glm::vec3(1.0f));
			material->setColor3("material.specular", glm::vec3(1.0f));
			material->setFloat("material.shininess", 64.0f);

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
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);

			std::string name = "SphereMat" + std::to_string(materialCount);
			std::shared_ptr<Material> material = std::make_shared<Material>(name,
				AssetManager::getShader("PrimitiveMaterial"));

			material->setColor3("material.diffuse", glm::vec3(1.0f));
			material->setColor3("material.ambient", glm::vec3(1.0f));
			material->setColor3("material.specular", glm::vec3(1.0f));
			material->setFloat("material.shininess", 64.0f);

			AssetManager::AddMaterial(name , material);
			ComponentManager::AddComponent<RenderComponent>(newEntity->Id, "Sphere", name);
			materialCount++;
			return newEntity;
		}
		
		static std::shared_ptr<Entity> CreateSuzane(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Suzane");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			std::string name = "Suzane" + std::to_string(materialCount);
			std::shared_ptr<Material> material = std::make_shared<Material>(name,
				AssetManager::getShader("PrimitiveMaterial"));

			material->setColor3("material.diffuse", glm::vec3(1.0f));
			material->setColor3("material.ambient", glm::vec3(1.0f));
			material->setColor3("material.specular", glm::vec3(1.0f));
			material->setFloat("material.shininess", 64.0f);

			AssetManager::AddMaterial(name, material);
			ComponentManager::AddComponent<RenderComponent>(newEntity->Id, "Suzane", name);
			materialCount++;
			return newEntity;
		}

		static std::shared_ptr<Entity> CreateCylinder(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Cylinder");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			std::string name = "Cylinder" + std::to_string(materialCount);
			std::shared_ptr<Material> material = std::make_shared<Material>(name,
				AssetManager::getShader("PrimitiveMaterial"));

			material->setColor3("material.diffuse", glm::vec3(1.0f));
			material->setColor3("material.ambient", glm::vec3(1.0f));
			material->setColor3("material.specular", glm::vec3(1.0f));
			material->setFloat("material.shininess", 64.0f);

			AssetManager::AddMaterial(name, material);
			ComponentManager::AddComponent<RenderComponent>(newEntity->Id, "Cylinder", name);
			materialCount++;
			return newEntity;
		}
		
		static std::shared_ptr<Entity> CreateTorus(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Torus");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
			std::string name = "Torus" + std::to_string(materialCount);
			std::shared_ptr<Material> material = std::make_shared<Material>(name,
				AssetManager::getShader("PrimitiveMaterial"));

			material->setColor3("material.diffuse", glm::vec3(1.0f));
			material->setColor3("material.ambient", glm::vec3(1.0f));
			material->setColor3("material.specular", glm::vec3(1.0f));
			material->setFloat("material.shininess", 64.0f);

			AssetManager::AddMaterial(name, material);
			ComponentManager::AddComponent<RenderComponent>(newEntity->Id, "Torus", name);
			materialCount++;
			return newEntity;
		}

	private:
		static int materialCount;
	};

}

#endif