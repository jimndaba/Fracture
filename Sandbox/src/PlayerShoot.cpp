#include "PlayerShoot.h"
#include "BulletScript.h"

Fracture::PlayerShootScript::PlayerShootScript(uint32_t id, std::shared_ptr<Entity> root):GameLogic(),m_id(id),m_root(root)
{
}

Fracture::PlayerShootScript::~PlayerShootScript()
{
}

void Fracture::PlayerShootScript::onStart()
{
	transform = GetComponent<TransformComponent>(m_id);
}

void Fracture::PlayerShootScript::onUpdate(float dt)
{
	

	if (InputManager::IsKeyDown(KeyCode::Space) | InputManager::IsMouseDown(MOUSECODE::ButtonLeft))
	{
		if (canShoot)
		{
		
			//FRACTURE_INFO("SHOT");
			std::shared_ptr<Fracture::Entity> m_bullet = Fracture::EntityManager::CreateEntity<Fracture::Entity>();
			m_bullet->name = "Enemy";

			std::shared_ptr<Fracture::RelationShipComponent> realationship = std::shared_ptr<Fracture::RelationShipComponent>(new Fracture::RelationShipComponent(m_bullet->Id));
			realationship->SetParent(m_root);

			std::shared_ptr<Fracture::TransformComponent> bulletTransform = std::make_shared<Fracture::TransformComponent>(m_bullet->Id, transform->Position, glm::vec3(1.00f), glm::vec3(0.0f));

			std::shared_ptr<Fracture::BulletScript> bulletScript = std::make_shared<Fracture::BulletScript>(m_bullet->Id,transform->Position);
			bulletScript->bulletSpeed = 60.0f;

			Fracture::ComponentManager::AddComponent(realationship);
			Fracture::ComponentManager::AddComponent(bulletTransform);
			Fracture::ComponentManager::AddComponent<Fracture::RenderComponent>(m_bullet->Id, "bullet", "default");
			Fracture::ComponentManager::AddComponent<Fracture::TagComponent>(m_bullet->Id);
			Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(m_bullet->Id, bulletScript);
			Fracture::ComponentManager::AddComponent<Fracture::BoxColliderComponent>(m_bullet->Id, 1, 1, 1);
			std::shared_ptr<RigidBodyComponent> bulletRb = std::make_shared<RigidBodyComponent>(m_bullet->Id, 1.0f);
			bulletRb->collisionGroup = CollisionGroup::COLGROUP_Bullet;
			bulletRb->collisionMask = CollisionMask::BulletMask;
			Fracture::ComponentManager::AddComponent(bulletRb);

			Game::addEntity(m_bullet);

			currenttime = 0.0f;
			canShoot = false;
		}

	}
	currenttime += dt;
	if (currenttime > cooldown)
	{
		//FRACTURE_INFO("CAN SHOOT");
		canShoot = true;
	}
}

void Fracture::PlayerShootScript::onLateUpdate(float dt)
{
}

void Fracture::PlayerShootScript::onCollision(CollisionEvent* collision)
{
}


