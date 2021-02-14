#include "AnimatorComponent.h"
#include "Component/ComponentManager.h"
#include "Component/RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimationClip.h"
#include "glm/ext/quaternion_common.hpp"

Fracture::AnimatorComponent::AnimatorComponent(uint32_t entityID):Component(entityID,ComponentType::None)
{

}

void Fracture::AnimatorComponent::onStart()
{
}

void Fracture::AnimatorComponent::SetAnimation(const std::string& name)
{
    std::shared_ptr<AnimationClip> animation = m_animations[name];

    if (animation)
    {
        m_CurrentAnimation = animation;
    }
    else
    {
        FRACTURE_ERROR("Couldnt find animation: {}", name);
    }
}

void Fracture::AnimatorComponent::Accept(ISceneProbe* visitor)
{
}

void Fracture::AnimatorComponent::Accept(ISceneProbe* visitor,float dt)
{
    visitor->VisitAnimatorComponent(std::shared_ptr<AnimatorComponent>(this),dt);
}
