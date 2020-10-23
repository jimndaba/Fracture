#include "Fracture.h"
#include "SandboxScene.h"
#include "Component/TagComponent.h"
#include "CameraFollowScript.h"
#include "PlayerShoot.h"
#include "EnemySpawner.h"
#include "PlayerScript.h"
#include "Enemy.h"

SandboxScene::SandboxScene():Fracture::Scene()
{
}

SandboxScene::~SandboxScene()
{
}

void SandboxScene::onLoad()
{
	//models
	Fracture::AssetManager::AddModel("player", "bin/content/models/playerPlane/playerplane.fbx");
	Fracture::AssetManager::AddModel("enemy", "bin/content/models/enemyPlane/enemyplane.fbx");
	Fracture::AssetManager::AddModel("bullet", "bin/content/models/bullet/bullet.fbx");

	//textures
	Fracture::AssetManager::AddShader("default", "bin/content/shaders/model/vertex.glsl", "bin/content/shaders/model/fragment.glsl");

	std::shared_ptr<Fracture::Material> defaultMaterial = std::shared_ptr<Fracture::Material>(new Fracture::Material("default", Fracture::AssetManager::getShader("default")));


	defaultMaterial->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	defaultMaterial->setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
	defaultMaterial->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	defaultMaterial->setVec3("light.specular", 1.0f, 1.0f, 1.0f);	

	// material properties
	defaultMaterial->setFloat("material.shininess", 64.0f);


	Fracture::AssetManager::AddMaterial("default", defaultMaterial);
	

	std::shared_ptr<Fracture::Entity> player = Fracture::EntityManager::CreateEntity<Fracture::Entity>();
	player->name = "Player";
	
	std::shared_ptr<Fracture::RelationShipComponent> realationship = std::shared_ptr<Fracture::RelationShipComponent>(new Fracture::RelationShipComponent(player->Id));
	realationship->SetParent(Root());
	Fracture::ComponentManager::AddComponent(realationship);
	std::shared_ptr<Fracture::TransformComponent> p1_Transform = std::make_shared<Fracture::TransformComponent>(player->Id, glm::vec3(0.0f), glm::vec3(0.03f), glm::vec3(0.0f));
	Fracture::ComponentManager::AddComponent<Fracture::RenderComponent>(player->Id, "player", "default");
	Fracture::ComponentManager::AddComponent<Fracture::TagComponent>(player->Id);

	std::shared_ptr<Fracture::PlayerScript> playerScript = std::make_shared<Fracture::PlayerScript>(player->Id);
	playerScript->playerSpeed = 25.0f;

	std::shared_ptr<Fracture::PlayerShootScript> shootScript = std::make_shared<Fracture::PlayerShootScript>(player->Id,Root());
	Fracture::ComponentManager::AddComponent(p1_Transform);
	Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(player->Id, playerScript);
	Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(player->Id, shootScript);	
	
	Fracture::ComponentManager::AddComponent<Fracture::BoxColliderComponent>(player->Id, 1, 1, 1);

	std::shared_ptr<Fracture::RigidBodyComponent> playerRb = std::make_shared<Fracture::RigidBodyComponent>(player->Id, 1.0f);
	playerRb->collisionGroup = Fracture::CollisionGroup::COLGROUP_Player;
	playerRb->collisionMask = Fracture::CollisionMask::PlayerMask;
	Fracture::ComponentManager::AddComponent(playerRb);


	std::shared_ptr<Fracture::CameraFollowScript> cameraFollow = std::make_shared<Fracture::CameraFollowScript>(MainCamera()->Id);
	cameraFollow->target = player->Id;
	cameraFollow->smoothTime = 0.95f;
	cameraFollow->offset = glm::vec3(0.0f, -30.0f,50.0f);

	Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(MainCamera()->Id,cameraFollow);

	addEntity(player);

	std::shared_ptr<Fracture::Entity> Spawner = Fracture::EntityManager::CreateEntity<Fracture::Entity>();
	player->name = "Player";
	std::shared_ptr<Fracture::EnemySpawner> spawnerScript = std::make_shared<Fracture::EnemySpawner>(player->Id,Root());
	spawnerScript->spawnRate = 2;
	Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(Spawner->Id, spawnerScript);

	Fracture::ComponentManager::RemoveComponent<Fracture::TagComponent>(player->Id);

	

}
