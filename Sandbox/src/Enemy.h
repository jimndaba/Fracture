#pragma once

#include "Fracture.h"
#include "EnemyScript.h"

namespace Fracture
{


	class Enemy
	{

	public:
		Enemy(std::shared_ptr<Entity> parent,  glm::vec3 position)
		{
			std::shared_ptr<Fracture::Entity> m_enemy = Fracture::EntityManager::CreateEntity<Fracture::Entity>();
			m_enemy->name = "Enemy2";

			std::shared_ptr<Fracture::RelationShipComponent> realationship = std::shared_ptr<Fracture::RelationShipComponent>(new Fracture::RelationShipComponent(m_enemy->Id));
			realationship->SetParent(parent);
			
			std::shared_ptr<Fracture::TransformComponent> p1_Transform = std::make_shared<Fracture::TransformComponent>(m_enemy->Id, position, glm::vec3(0.03f), glm::vec3(0.0f));
			
			std::shared_ptr<Fracture::EnemyScript> enemyScript = std::make_shared<Fracture::EnemyScript>(m_enemy->Id);
			enemyScript->EnemySpeed = 10.0f;
			
			Fracture::ComponentManager::AddComponent(realationship);
			Fracture::ComponentManager::AddComponent<Fracture::RenderComponent>(m_enemy->Id, "player", "default");
			Fracture::ComponentManager::AddComponent<Fracture::TagComponent>(m_enemy->Id);
			Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(m_enemy->Id, enemyScript);
			Fracture::ComponentManager::AddComponent(p1_Transform);
			Fracture::ComponentManager::AddComponent<Fracture::BoxColliderComponent>(m_enemy->Id, 1, 1, 1);
			Fracture::ComponentManager::AddComponent<Fracture::RigidBodyComponent>(m_enemy->Id, 0.0f);

		}

		std::shared_ptr<Fracture::Entity> m_enemy;


	};

}