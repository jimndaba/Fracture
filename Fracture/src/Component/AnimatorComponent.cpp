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
   // auto component = std::make_shared<AnimatorComponent>(*this);
    visitor->VisitAnimatorComponent(this,dt);
}

std::vector<glm::mat4> Fracture::AnimatorComponent::getAnimationTransforms()
{
    return m_Transforms;
}

void Fracture::AnimatorComponent::resizeTransforms()
{
    m_Transforms.resize(m_skeleton->NumBones);
}

void Fracture::AnimatorComponent::pushTransform()
{
    for (int i = 0; i < m_skeleton->NumBones; i++)
    {
        m_Transforms[i] = m_skeleton->m_BoneInfo[i].FinalTransformation;
    }
   
}
