#include "AnimatorProbe.h"
#include "AnimationManager.h"
#include "Component/ComponentManager.h"
#include "Component/AnimatorComponent.h"

Fracture::AnimatorProbe::AnimatorProbe(AnimationManager& manager):m_manager(manager)
{
}

void Fracture::AnimatorProbe::VisitAnimatorComponent(AnimatorComponent* component,float dt)
{
	ProfilerTimer timer("Visit Render Comp");
	std::shared_ptr<AnimatorComponent> animator = ComponentManager::GetComponent<AnimatorComponent>(component->EntityID);
	m_manager.BoneTransformation(dt, animator->m_skeleton, animator->m_Transforms);
}
