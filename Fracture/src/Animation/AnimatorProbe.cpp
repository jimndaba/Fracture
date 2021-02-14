#include "AnimatorProbe.h"
#include "AnimationManager.h"
#include "Component/ComponentManager.h"
#include "Component/AnimatorComponent.h"

Fracture::AnimatorProbe::AnimatorProbe(AnimationManager& manager):m_manager(manager)
{
}

void Fracture::AnimatorProbe::VisitAnimatorComponent(std::shared_ptr<AnimatorComponent>& component,float dt)
{
	ProfilerTimer timer("Visit Render Comp");
	m_manager.BoneTransformation(dt, component, component->m_Transforms);

	if (ComponentManager::HasComponent<TransformComponent>(component->EntityID))
	{
		auto transform = ComponentManager::GetComponent<TransformComponent>(component->EntityID);
		m_manager.AnimateTransform(dt, component, transform);
	}
	
	if (ComponentManager::HasComponent<RenderComponent>(component->EntityID))
	{
		auto renderer = ComponentManager::GetComponent<RenderComponent>(component->EntityID);
		m_manager.AnimateRenderer(dt,component,renderer);
	}
	
}
