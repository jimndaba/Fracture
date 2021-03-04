#include "ComponentManager.h"
#include "Component.h"
#include "ComponentSet.h"

#include "TagComponent.h"
#include "IUpdatable.h"
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
#include "Physics/PhysicsManager.h"
#include "AnimatorComponent.h"
#include "Game/Game.h"

std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_tagComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_RelationshipComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_TransformerComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_CameraControllerComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_RenderComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_LightComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_RigidBodyComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_BoxColliderComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_ScriptComponents;
std::unique_ptr<Fracture::ComponentSet> Fracture::ComponentManager::m_BillboardComponents;

std::map<std::type_index, std::unique_ptr<Fracture::ComponentSet>> Fracture::ComponentManager::Register;

Fracture::ComponentManager::ComponentManager()
{

}

void Fracture::ComponentManager::onInit()
{
	Register[typeid(TagComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(RelationShipComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(TransformComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(CameraControllerComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(RenderComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(BillboardComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(LightComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(RigidBodyComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(BoxColliderComponent)] = std::make_unique<ComponentSet>();
	Register[typeid(ScriptComponent)] = std::make_unique<ComponentSet>();	
	Register[typeid(AnimatorComponent)] = std::make_unique<ComponentSet>();
}

void Fracture::ComponentManager::onLoad()
{	
	
	for (auto& component : GetAllComponents<BoxColliderComponent>())
	{		
		std::shared_ptr<BoxColliderComponent> c = std::dynamic_pointer_cast<BoxColliderComponent>(component);
		PhysicsManager::AddCollider(c->GetID(),c->m_boxCollider.get());
	}

	for (auto& component : GetAllComponents<RigidBodyComponent>())
	{
		std::shared_ptr<RigidBodyComponent> c = std::dynamic_pointer_cast<RigidBodyComponent>(component);
		PhysicsManager::AddRigidBody(c->GetID(), c->m_rigid.get(), c->collisionGroup, c->collisionMask);
	}
}

const void Fracture::ComponentManager::onUpdate(float dt)
{	
	ProfilerTimer timer("Component OnUpdate");
	for (auto& component : GetAllComponents<CameraControllerComponent>())
	{
		std::shared_ptr<CameraControllerComponent> c = std::dynamic_pointer_cast<CameraControllerComponent>(component);
		c->onUpdate(dt);			
	}	
}

const void Fracture::ComponentManager::ClearComponents()
{
	for (auto it = Register.begin(); it != Register.end(); ++it)
	{
		it->second->Clear();
	}
}

const void Fracture::ComponentManager::RemoveComponentsbyID(const UUID& id)
{	
	for (auto it = Register.begin(); it != Register.end(); ++it)
	{
		it->second->RemoveComponent(id);
	}
}





