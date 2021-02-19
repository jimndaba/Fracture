#include "AnimatorProbe.h"
#include "AnimationManager.h"
#include "Component/ComponentManager.h"
#include "Component/AnimatorComponent.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Animation/AnimationClip.h"

Fracture::AnimatorProbe::AnimatorProbe(AnimationManager& manager):m_manager(manager)
{
}

void Fracture::AnimatorProbe::VisitAnimatorComponent(AnimatorComponent* component,float dt)
{
	ProfilerTimer timer("Visit Animator Comp");
	if(component->m_CurrentAnimation)
	{ 
		for (int i = 0; i < component->m_CurrentAnimation->m_channels.size(); i++)
		{
			m_manager.BoneTransformation(component->m_CurrentAnimation->m_channels[i], component->m_skeleton, component->m_CurrentAnimation->AnimationTime);
		}

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

		m_manager.UpdateFinalTransforms(component->m_skeleton);
		m_manager.PushTransforms(component);

	}
}
