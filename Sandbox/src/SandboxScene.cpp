#include "SandboxScene.h"


SandboxScene::SandboxScene()
{
}

SandboxScene::~SandboxScene()
{
}

void SandboxScene::onLoad()
{
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	};
	//models
	Fracture::AssetManager::AddModel("samus", "content/models/samus/DolSzerosuitR1.obj");
	Fracture::AssetManager::AddModel("cube", "content/models/cube.obj");

	//textures
	Fracture::AssetManager::AddTexture("container", "content/textures/container.png", Fracture::TextureType::Diffuse);
	Fracture::AssetManager::AddTexture("specular", "content/textures/container_specular.png", Fracture::TextureType::Specular);

	Fracture::AssetManager::AddShader("default", "content/shaders/model/vertex.glsl", "content/shaders/model/fragment.glsl");

	std::shared_ptr<Fracture::Material> defaultMaterial = std::shared_ptr<Fracture::Material>(new Fracture::Material("default", Fracture::AssetManager::getShader("default")));

	std::shared_ptr<Fracture::Material> samusMaterial = std::shared_ptr<Fracture::Material>(new Fracture::Material("samus", Fracture::AssetManager::getShader("default")));

	defaultMaterial->SetTexture("material.diffuse", Fracture::AssetManager::getTexture("container"), 0);
	defaultMaterial->SetTexture("material.specular", Fracture::AssetManager::getTexture("specular"), 1);


	defaultMaterial->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	defaultMaterial->setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
	defaultMaterial->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	defaultMaterial->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

	samusMaterial->setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	samusMaterial->setVec3("light.ambient", 0.6f, 0.6f, 0.6f);
	samusMaterial->setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
	samusMaterial->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	samusMaterial->setFloat("material.shininess", 64.0f);

	// material properties
	defaultMaterial->setFloat("material.shininess", 64.0f);


	Fracture::AssetManager::AddMaterial("default", defaultMaterial);
	Fracture::AssetManager::AddMaterial("samus", samusMaterial);

	std::shared_ptr<Fracture::Entity> monkey = Fracture::EntityManager::CreateEntity<Fracture::Entity>();
	Fracture::ComponentManager::AddComponent<Fracture::TransformComponent>(monkey->Id, glm::vec3(0.0f), glm::vec3(0.3f));
	Fracture::ComponentManager::AddComponent<Fracture::RenderComponent>(monkey->Id, "samus", "samus");
	addEntity(monkey);

	for (unsigned int i = 0; i < 10; i++)
	{
		std::shared_ptr<Fracture::Entity> cube = Fracture::EntityManager::CreateEntity<Fracture::Entity>();
		cube->name = "cube";
		Fracture::ComponentManager::AddComponent<Fracture::TransformComponent>(cube->Id, cubePositions[i]);
		Fracture::ComponentManager::AddComponent<Fracture::RenderComponent>(cube->Id, "cube", "default");
		//addEntity(cube);
		monkey->addChild(cube);
	}




}
