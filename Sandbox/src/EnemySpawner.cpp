#include "EnemySpawner.h"
#include "EnemyScript.h"


Fracture::EnemySpawner::EnemySpawner(uint32_t id, std::shared_ptr<Entity> root):GameLogic(),m_root(root)
{
}

Fracture::EnemySpawner::~EnemySpawner()
{
}

void Fracture::EnemySpawner::onStart()
{


}

void Fracture::EnemySpawner::onUpdate(float dt)
{

	counter += dt;

	if (counter > spawnRate)
	{
		int random = -50 + (std::rand() % (50 - -25 + 1));
		glm::vec3 position = glm::vec3(random,100,0.0f);


		std::shared_ptr<Fracture::Entity> m_enemy = Fracture::EntityManager::CreateEntity<Fracture::Entity>();
		m_enemy->name = "Enemy";

		std::shared_ptr<Fracture::RelationShipComponent> realationship = std::shared_ptr<Fracture::RelationShipComponent>(new Fracture::RelationShipComponent(m_enemy->Id));
		realationship->SetParent(m_root);

		std::shared_ptr<Fracture::TransformComponent> p1_Transform = std::make_shared<Fracture::TransformComponent>(m_enemy->Id, position, glm::vec3(0.03f), glm::vec3(0.0f));

		std::shared_ptr<Fracture::EnemyScript> enemyScript = std::make_shared<Fracture::EnemyScript>(m_enemy->Id);
		enemyScript->EnemySpeed = 8.0f;

		Fracture::ComponentManager::AddComponent(realationship);
		Fracture::ComponentManager::AddComponent(p1_Transform);
		Fracture::ComponentManager::AddComponent<Fracture::RenderComponent>(m_enemy->Id, "enemy", "default");
		Fracture::ComponentManager::AddComponent<Fracture::TagComponent>(m_enemy->Id);
		Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(m_enemy->Id, enemyScript);		
		Fracture::ComponentManager::AddComponent<Fracture::BoxColliderComponent>(m_enemy->Id, 1, 1, 1);
		std::shared_ptr<RigidBodyComponent> enemyRb = std::make_shared<RigidBodyComponent>(m_enemy->Id, 1.0f);
		enemyRb->collisionGroup = CollisionGroup::COLGROUP_Enemy;
		enemyRb->collisionMask = CollisionMask::EnemyMask;
		Fracture::ComponentManager::AddComponent(enemyRb);
		Game::addEntity(m_enemy);
		counter = 0;
	}

}

void Fracture::EnemySpawner::onLateUpdate(float dt)
{
}

void Fracture::EnemySpawner::onCollision(CollisionEvent* collision)
{
}
