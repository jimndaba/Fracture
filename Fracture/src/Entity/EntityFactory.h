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
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Scene/Scene.h"
#include "Logging/Logger.h"

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


			std::shared_ptr<Model> model = AssetManager::getModel("Cube");

			if (model)
			{
				for (auto mesh : model->GetMeshes())
				{
					std::shared_ptr<Entity> shape = EntityManager::CreateEntity<Entity>();
					ComponentManager::AddComponent<TagComponent>(shape->Id, "CubeMesh");
					std::shared_ptr<RelationShipComponent> m_relationship = std::make_shared<RelationShipComponent>(shape->Id);
					m_relationship->SetParent(newEntity->Id);
					ComponentManager::AddComponent<RelationShipComponent>(m_relationship);
					ComponentManager::AddComponent<TransformComponent>(shape->Id);


					std::string name = mesh->MaterialName;
					std::shared_ptr<Material> material = std::make_shared<Material>(name,
						AssetManager::getShader("PrimitiveMaterial"));

					material->setColor3("material.diffuse", glm::vec3(1.0f));
					material->setColor3("material.ambient", glm::vec3(1.0f));
					material->setColor3("material.specular", glm::vec3(1.0f));
					material->setFloat("material.shininess", 64.0f);

					AssetManager::AddMaterial(name, material);
					ComponentManager::AddComponent<RenderComponent>(shape->Id, mesh, name);
					scene->addEntity(shape);
					materialCount++;
				}
			}

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
			
			std::shared_ptr<Model> model = AssetManager::getModel("Plane");

			if (model)
			{
				for (auto mesh : model->GetMeshes())
				{
					std::shared_ptr<Entity> shape = EntityManager::CreateEntity<Entity>();
					ComponentManager::AddComponent<TagComponent>(shape->Id, "PlaneMesh");
					std::shared_ptr<RelationShipComponent> m_relationship = std::make_shared<RelationShipComponent>(shape->Id);
					m_relationship->SetParent(newEntity->Id);
					ComponentManager::AddComponent<RelationShipComponent>(m_relationship);
					ComponentManager::AddComponent<TransformComponent>(shape->Id);


					std::string name = mesh->MaterialName;
					std::shared_ptr<Material> material = std::make_shared<Material>(name,
						AssetManager::getShader("PrimitiveMaterial"));

					material->setColor3("material.diffuse", glm::vec3(1.0f));
					material->setColor3("material.ambient", glm::vec3(1.0f));
					material->setColor3("material.specular", glm::vec3(1.0f));
					material->setFloat("material.shininess", 64.0f);

					AssetManager::AddMaterial(name, material);
					ComponentManager::AddComponent<RenderComponent>(shape->Id, mesh, name);
					scene->addEntity(shape);
					materialCount++;
				}
			}
			
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

			std::shared_ptr<Model> model = AssetManager::getModel("Sphere");

			if (model)
			{
				for (auto mesh : model->GetMeshes())
				{
					std::shared_ptr<Entity> shape = EntityManager::CreateEntity<Entity>();
					ComponentManager::AddComponent<TagComponent>(shape->Id, "SphereMesh");
					std::shared_ptr<RelationShipComponent> m_relationship = std::make_shared<RelationShipComponent>(shape->Id);
					m_relationship->SetParent(newEntity->Id);
					ComponentManager::AddComponent<RelationShipComponent>(m_relationship);
					ComponentManager::AddComponent<TransformComponent>(shape->Id);
					ComponentManager::AddComponent<RenderComponent>(shape->Id, mesh, mesh->MaterialName);
					scene->addEntity(shape);
					materialCount++;
				}
			}
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

			std::shared_ptr<Model> model = AssetManager::getModel("Suzane");

			if (model)
			{
				for (auto mesh : model->GetMeshes())
				{
					std::shared_ptr<Entity> shape = EntityManager::CreateEntity<Entity>();
					ComponentManager::AddComponent<TagComponent>(shape->Id, "SuzaneMesh");
					std::shared_ptr<RelationShipComponent> m_relationship = std::make_shared<RelationShipComponent>(shape->Id);
					m_relationship->SetParent(newEntity->Id);
					ComponentManager::AddComponent<RelationShipComponent>(m_relationship);
					ComponentManager::AddComponent<TransformComponent>(shape->Id);
					ComponentManager::AddComponent<RenderComponent>(shape->Id, mesh, mesh->MaterialName);
					scene->addEntity(shape);
					materialCount++;
				}
			}
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
			
			std::shared_ptr<Model> model = AssetManager::getModel("Cylinder");

			if (model)
			{
				for (auto mesh : model->GetMeshes())
				{
					std::shared_ptr<Entity> shape = EntityManager::CreateEntity<Entity>();
					ComponentManager::AddComponent<TagComponent>(shape->Id, "CylinderMesh");
					std::shared_ptr<RelationShipComponent> m_relationship = std::make_shared<RelationShipComponent>(shape->Id);
					m_relationship->SetParent(newEntity->Id);
					ComponentManager::AddComponent<RelationShipComponent>(m_relationship);
					ComponentManager::AddComponent<TransformComponent>(shape->Id);
					ComponentManager::AddComponent<RenderComponent>(shape->Id, mesh, mesh->MaterialName);
					scene->addEntity(shape);
					materialCount++;
				}
			}
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
			std::shared_ptr<Model> model = AssetManager::getModel("Torus");

			if (model)
			{
				for (auto mesh : model->GetMeshes())
				{
					std::shared_ptr<Entity> shape = EntityManager::CreateEntity<Entity>();
					ComponentManager::AddComponent<TagComponent>(shape->Id, "TorusMesh");
					std::shared_ptr<RelationShipComponent> m_relationship = std::make_shared<RelationShipComponent>(shape->Id);
					m_relationship->SetParent(newEntity->Id);
					ComponentManager::AddComponent<RelationShipComponent>(m_relationship);
					ComponentManager::AddComponent<TransformComponent>(shape->Id);
					ComponentManager::AddComponent<RenderComponent>(shape->Id, mesh, mesh->MaterialName);
					scene->addEntity(shape);
					materialCount++;
				}
			}
			return newEntity;
		}

		static std::shared_ptr<Entity> CreateModel(std::shared_ptr<Scene> scene, std::string ModelName)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, ModelName);
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
		
			std::shared_ptr<Model> model = AssetManager::getModel(ModelName);

			if (model)
			{
				for (auto mesh : model->GetMeshes())
				{
					std::shared_ptr<Entity> childMesh = EntityManager::CreateEntity<Entity>();
					ComponentManager::AddComponent<TagComponent>(childMesh->Id, mesh->Name);
					std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(childMesh->Id);
					relationship->SetParent(newEntity->Id);
					ComponentManager::AddComponent<RelationShipComponent>(relationship);
					ComponentManager::AddComponent<TransformComponent>(childMesh->Id,mesh->position, mesh->scale, mesh->rotation);
					ComponentManager::AddComponent<RenderComponent>(childMesh->Id,mesh, mesh->MaterialName);
					scene->addEntity(childMesh);
				}
			}
			else
			{
				FRACTURE_ERROR("FAILED TO CREATE MODEL: {}", ModelName);
			}
			
			return newEntity;
		}

	private:
		static int materialCount;
	};

}

#endif