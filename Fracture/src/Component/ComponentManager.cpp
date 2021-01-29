#include "ComponentManager.h"
#include "Component.h"
#include "TagComponent.h"
#include "IUpdatable.h"
#include "Game/Game.h"
#include "Scripting/ScriptManager.h"
#include "Rendering/Renderer.h"
#include "ScriptComponent.h"
#include "BoxColliderComponent.h"
#include "RigidBodyComponent.h"
#include "RelationshipComponent.h"
#include "TransformComponent.h"
#include "LightComponent.h"
#include "CameraControllerComponent.h"
#include "BillboardComponent.h"
#include "RelationshipComponent.h"
#include "RenderComponent.h"
#include <iostream>
#include "Profiling/Profiler.h"


std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_tagComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_RelationshipComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_TransformerComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_CameraControllerComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_RenderComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_LightComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_RigidBodyComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_BoxColliderComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_ScriptComponents;
std::shared_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_BillboardComponents;

std::map<std::type_index, std::shared_ptr<Fracture::ComponentSet>> Fracture::ComponentManager::Register;

Fracture::ComponentManager::ComponentManager()
{

}

Fracture::ComponentManager::~ComponentManager()
{
	
}

void Fracture::ComponentManager::onInit()
{
	m_tagComponents				 = std::make_shared<ComponentSet>();
	m_RelationshipComponents	 = std::make_shared<ComponentSet>();
	m_TransformerComponents		 = std::make_shared<ComponentSet>();
	m_CameraControllerComponents = std::make_shared<ComponentSet>();
	m_RenderComponents			 = std::make_shared<ComponentSet>();
	m_LightComponents			 = std::make_shared<ComponentSet>();
	m_RigidBodyComponents		 = std::make_shared<ComponentSet>();
	m_BoxColliderComponents		 = std::make_shared<ComponentSet>();
	m_ScriptComponents			 = std::make_shared<ComponentSet>();
	m_BillboardComponents		 = std::make_shared<ComponentSet>();

	Register.emplace(typeid(TagComponent),m_tagComponents);
	Register.emplace(typeid(RelationShipComponent), m_RelationshipComponents);
	Register.emplace(typeid(TransformComponent), m_TransformerComponents);
	Register.emplace(typeid(CameraControllerComponent), m_CameraControllerComponents);
	Register.emplace(typeid(RenderComponent), m_RenderComponents);
	Register.emplace(typeid(BillboardComponent), m_BillboardComponents);
	Register.emplace(typeid(LightComponent), m_LightComponents);
	Register.emplace(typeid(RigidBodyComponent), m_RigidBodyComponents);
	Register.emplace(typeid(BoxColliderComponent), m_BoxColliderComponents);
	Register.emplace(typeid(ScriptComponent), m_ScriptComponents);

	
}

void Fracture::ComponentManager::onLoad()
{	
	for (auto& component :m_BoxColliderComponents->Components())
	{		
		std::shared_ptr<BoxColliderComponent> c = std::dynamic_pointer_cast<BoxColliderComponent>(component);
		PhysicsManager::AddCollider(c->EntityID,c->m_boxCollider);
	}

	for (auto& component : m_RigidBodyComponents->Components())
	{
		std::shared_ptr<RigidBodyComponent> c = std::dynamic_pointer_cast<RigidBodyComponent>(component);
		PhysicsManager::AddRigidBody(c->EntityID, c->m_rigid, c->collisionGroup, c->collisionMask);
	}
}

void Fracture::ComponentManager::onUpdate(float dt)
{	

	ProfilerTimer timer("Component OnUpdate");
	for (auto& component :m_CameraControllerComponents->Components())
	{
		std::shared_ptr<CameraControllerComponent> c = std::dynamic_pointer_cast<CameraControllerComponent>(component);
		c->onUpdate(dt);			
	}

	for (auto& component : m_ScriptComponents->Components())
	{	
		std::shared_ptr<ScriptComponent> c = std::dynamic_pointer_cast<ScriptComponent>(component);
		if(c)
			Game::AddScript(c->GetScript());
	}
	for (auto& component : m_RenderComponents->Components())
	{
		std::shared_ptr<RenderComponent> c = std::dynamic_pointer_cast<RenderComponent>(component);
		if (c)
		{
			//Renderer::getInstance()->PushCommand(component->model,com)
		}
			
	}	
}

void Fracture::ComponentManager::onDebugDraw()
{
	/*
	for (auto& component : m_tagComponents)
	{
		component->OnDebug();
	}
	for (auto& component : m_RelationshipComponents)
	{
		component->OnDebug();
	}
	for (auto& component : m_TransformerComponents)
	{
		component->OnDebug();
	}
	for (auto& component : m_CameraControllerComponents)
	{
		component->OnDebug();
	}
	for (auto& component : m_LightComponents)
	{
		component->OnDebug();
	}
	for (auto& component : m_RigidBodyComponents)
	{
		component->OnDebug();
	}
	for (auto& component : m_BoxColliderComponents)
	{
		component->OnDebug();
	}
	for (auto& component : m_ScriptComponents)
	{
		component->OnDebug();
	}
	for (auto& component : m_EditorNodeComponents)
	{
		component->OnDebug();
	}
	*/

}

void Fracture::ComponentManager::ClearComponents()
{
	for (auto it = Register.begin(); it != Register.end(); ++it)
	{
		it->second->Clear();
	}
}

void Fracture::ComponentManager::RemoveComponentsbyID(uint32_t id)
{	
	for (auto it = Register.begin(); it != Register.end(); ++it)
	{
		it->second->RemoveComponent(id);
	}
}
