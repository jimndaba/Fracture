#pragma once
#ifndef ENTITYFACTORY_H
#define ENTITYFACTORY_H

#include <memory>
#include "Logging/Logger.h"
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
#include "Component/BillboardComponent.h"
#include "Component/AnimatorComponent.h"
#include "Animation/AnimationClip.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
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

			std::shared_ptr<TransformComponent> node = std::make_shared<TransformComponent
			>(newEntity->Id);

			node->setRotation(light->GetDirection());
			ComponentManager::AddComponent<TransformComponent>(node);
			

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
			ComponentManager::AddComponent<BillboardComponent>(newEntity->Id);
			ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
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

			std::shared_ptr<TransformComponent> node = std::make_shared<TransformComponent
			>(newEntity->Id);

			node->setPosition(light->GetPosition());
			node->setRotation(light->GetDirection());
			ComponentManager::AddComponent<TransformComponent>(node);

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
			std::shared_ptr<Model> model = AssetManager::getModel("Cube");
			
			if (model)
			{
				ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Cube");
				std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
				relationship->SetParent(scene->Root()->Id);
				ComponentManager::AddComponent<RelationShipComponent>(relationship);
				ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
				ComponentManager::AddComponent<RenderComponent>(newEntity->Id, model);					
			}
			else
			{
				FRACTURE_ERROR("FAILED TO CREATE MODEL: {}", model->Name);
				return nullptr;
			}

			return newEntity;
		}

		static std::shared_ptr<Entity> CreatePlane(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			std::shared_ptr<Model> model = AssetManager::getModel("Plane");

			if (model)
			{
				ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Plane");
				std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
				relationship->SetParent(scene->Root()->Id);
				ComponentManager::AddComponent<RelationShipComponent>(relationship);
				ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
				ComponentManager::AddComponent<RenderComponent>(newEntity->Id, model);
			}
			else
			{
				FRACTURE_ERROR("FAILED TO CREATE MODEL: {}", model->Name);
				return nullptr;
			}
			
			return newEntity;
		}
		
		static std::shared_ptr<Entity> CreateSphere(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Model> model = AssetManager::getModel("Sphere");
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			if (model)
			{
				for (auto mesh : model->GetMeshes())
				{
					
					ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Sphere");
					std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
					relationship->SetParent(scene->Root()->Id);

					ComponentManager::AddComponent<RelationShipComponent>(relationship);
					ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
					ComponentManager::AddComponent<RenderComponent>(newEntity->Id, model);
				}


				auto animator = std::make_shared<AnimatorComponent>(newEntity->Id);
				auto animation = std::make_shared<AnimationClip>();
				auto channel = AnimationChannel();

				channel.Name = "Color";

				AnimationKeyframe f1 = AnimationKeyframe();
				f1.Time = 0.0;
				f1.VEC4 = glm::vec4(0.5f,1.0f,0.8f,1.0f);
				channel.m_ColorKeys.push_back(f1);

				AnimationKeyframe f2 = AnimationKeyframe();
				f2.Time = 20.0;
				f2.VEC4 = glm::vec4(1.0f, 1.0f, 0.2f, 0.8f);
				channel.m_ColorKeys.push_back(f2);

				AnimationKeyframe f3 = AnimationKeyframe();
				f3.Time = 70.0;
				f3.VEC4 = glm::vec4(1.0f, 0.0f, 0.2f, 0.8f);
				channel.m_ColorKeys.push_back(f3);

				AnimationKeyframe f4 = AnimationKeyframe();
				f4.Time = 100.0;
				f4.VEC4 = glm::vec4(0.5f, 1.0f, 0.8f, 1.0f);
				channel.m_ColorKeys.push_back(f4);

				auto move = AnimationChannel();
				move.Name = "Position";

				AnimationKeyframe p1 = AnimationKeyframe();
				p1.Time = 0.0;
				p1.VEC3 = glm::vec3(0.0f, 0.0f, 0.0f);
				move.m_PositionKeys.push_back(p1);

				AnimationKeyframe p2 = AnimationKeyframe();
				p2.Time = 50.0;
				p2.VEC3 = glm::vec3(20.0f, 10.0f, -10.0f);
				move.m_PositionKeys.push_back(p2);

				AnimationKeyframe p3 = AnimationKeyframe();
				p3.Time = 100.0;
				p3.VEC3 = glm::vec3(0.0f, 0.0f, 0.0f);
				move.m_PositionKeys.push_back(p3);


				auto scale = AnimationChannel();
				scale.Name = "Scale";

				AnimationKeyframe s1 = AnimationKeyframe();
				s1.Time = 0.0;
				s1.VEC3 = glm::vec3(1.0f, 1.0f, 1.0f);
				scale.m_ScaleKeys.push_back(s1);

				AnimationKeyframe s2 = AnimationKeyframe();
				s2.Time = 60.0;
				s2.VEC3 = glm::vec3(2.0f, 2.0f, 2.0f);
				scale.m_ScaleKeys.push_back(s2);

				AnimationKeyframe s3 = AnimationKeyframe();
				s3.Time = 80.0;
				s3.VEC3 = glm::vec3(0.1f, 1.0f, 5.0f);
				scale.m_ScaleKeys.push_back(s3);

				AnimationKeyframe s4 = AnimationKeyframe();
				s4.Time = 100.0;
				s4.VEC3 = glm::vec3(1.0f, 1.0f, 1.0f);
				scale.m_ScaleKeys.push_back(s4);
				

				animation->NumberOfFrames = 100;
				animation->FramesPerSec = 30;
				animation->m_channels.push_back(channel);
				animation->m_channels.push_back(move);
				animation->m_channels.push_back(scale);

				animation->Name = "ChangeColor";
				animator->m_animations["ChangeColor"] = animation;
				animator->SetAnimation("ChangeColor");

				ComponentManager::AddComponent<AnimatorComponent>(animator);
			}
			else
			{
				FRACTURE_ERROR("FAILED TO CREATE MODEL: {}", model->Name);
				return nullptr;
			}
			return newEntity;
		}
		
		static std::shared_ptr<Entity> CreateSuzane(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			std::shared_ptr<Model> model = AssetManager::getModel("Suzane");

			if (model)
			{
				ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Suzane");
				std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
				relationship->SetParent(scene->Root()->Id);
				ComponentManager::AddComponent<RelationShipComponent>(relationship);
				ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
				ComponentManager::AddComponent<RenderComponent>(newEntity->Id, model);
			}
			else
			{
				FRACTURE_ERROR("FAILED TO CREATE MODEL: {}", model->Name);
				return nullptr;
			}
			return newEntity;
		}

		static std::shared_ptr<Entity> CreateCylinder(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			
			std::shared_ptr<Model> model = AssetManager::getModel("Cylinder");

			if (model)
			{
				ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Cylinder");
				std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
				relationship->SetParent(scene->Root()->Id);
				ComponentManager::AddComponent<RelationShipComponent>(relationship);
				ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
				ComponentManager::AddComponent<RenderComponent>(newEntity->Id, model);
			}
			else
			{
				FRACTURE_ERROR("FAILED TO CREATE MODEL: {}", model->Name);
				return nullptr;
			}
			return newEntity;
		}
		
		static std::shared_ptr<Entity> CreateTorus(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();			
			std::shared_ptr<Model> model = AssetManager::getModel("Torus");

			if (model)
			{
				ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Torus");
				std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
				relationship->SetParent(scene->Root()->Id);
				ComponentManager::AddComponent<RelationShipComponent>(relationship);
				ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
				ComponentManager::AddComponent<RenderComponent>(newEntity->Id, model);
			}
			else
			{
				FRACTURE_ERROR("FAILED TO CREATE MODEL: {}", model->Name);
				return nullptr;
			}
			return newEntity;
		}
	
		static std::shared_ptr<Entity> CreateCamera(std::shared_ptr<Scene> scene)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(newEntity->Id, "Camera");
			std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
			relationship->SetParent(scene->Root()->Id);
			ComponentManager::AddComponent<RelationShipComponent>(relationship);
			ComponentManager::AddComponent<CameraControllerComponent>(newEntity->Id);
			return newEntity;
		}

		static std::shared_ptr<Entity> CreateModel(std::shared_ptr<Scene> scene, std::string ModelName)
		{
			std::shared_ptr<Entity> newEntity = EntityManager::CreateEntity<Entity>();		
			std::shared_ptr<Model> model = AssetManager::getModel(ModelName);

			if (model)
			{
				ComponentManager::AddComponent<TagComponent>(newEntity->Id, ModelName);
				std::shared_ptr<RelationShipComponent> relationship = std::make_shared<RelationShipComponent>(newEntity->Id);
				relationship->SetParent(scene->Root()->Id);
				ComponentManager::AddComponent<RelationShipComponent>(relationship);
				ComponentManager::AddComponent<TransformComponent>(newEntity->Id);
				ComponentManager::AddComponent<RenderComponent>(newEntity->Id, model);
			}
			else
			{
				FRACTURE_ERROR("FAILED TO CREATE MODEL: {}", ModelName);
				return nullptr;
			}
			
			return newEntity;
		}

	private:
		static int materialCount;
	};

}

#endif