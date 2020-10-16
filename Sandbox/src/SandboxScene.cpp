#include "Fracture.h"
#include "SandboxScene.h"
#include "Component/TagComponent.h"
#include "CameraFollowScript.h"
#include "PlayerScript.h"

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
	Fracture::ComponentManager::AddComponent<Fracture::TransformComponent>(player->Id, glm::vec3(0.0f), glm::vec3(0.03f), glm::vec3(0.0f,0.50f,0));
	Fracture::ComponentManager::AddComponent<Fracture::RenderComponent>(player->Id, "player", "default");
	Fracture::ComponentManager::AddComponent<Fracture::TagComponent>(player->Id);
	Fracture::ComponentManager::AddComponent<Fracture::BoxColliderComponent>(player->Id,1,1,1);
	Fracture::ComponentManager::AddComponent<Fracture::RigidBodyComponent>(player->Id,0.0f);
	std::shared_ptr<Fracture::PlayerScript> playerScript = std::make_shared<Fracture::PlayerScript>(player->Id);
	playerScript->playerSpeed = 10.0f;

	Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(player->Id, playerScript);

	std::shared_ptr<Fracture::CameraFollowScript> cameraFollow = std::make_shared<Fracture::CameraFollowScript>(MainCamera()->Id);
	cameraFollow->target = player->Id;
	cameraFollow->smoothTime = 5.0f;
	cameraFollow->offset = glm::vec3(0.0f, -20.0f,15.0f);

	Fracture::ComponentManager::AddComponent<Fracture::ScriptComponent>(MainCamera()->Id,cameraFollow);

	addEntity(player);

}
