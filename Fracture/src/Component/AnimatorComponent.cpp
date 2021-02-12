#include "AnimatorComponent.h"
#include "Component/ComponentManager.h"
#include "Component/RenderComponent.h"
#include "Rendering/Model.h"
#include "Rendering/Mesh.h"
#include "Animation/AnimationClip.h"

Fracture::AnimatorComponent::AnimatorComponent(uint32_t entityID):Component(entityID,ComponentType::None),
m_meshes(ComponentManager::GetComponent<RenderComponent>(entityID)->GetModel()->m_meshes)
{
  //  m_meshes = ComponentManager::GetComponent<RenderComponent>(entityID)->GetModel()->m_meshes;
}

void Fracture::AnimatorComponent::onStart()
{
}

void Fracture::AnimatorComponent::OnUpdate(float dt)
{
    for (auto& mesh : m_meshes)
    {
        std::vector<glm::mat4> transforms;
        BoneTransformation(dt, mesh,transforms);
    }
}

glm::mat4 Fracture::AnimatorComponent::BoneTransformation(float dt, std::shared_ptr<Mesh>& mesh, std::vector<glm::mat4>& Transforms)
{
    glm::mat4 Identity = glm::identity<glm::mat4>();

    float TicksPerSecond = m_CurrentAnimation->FramesPerSec != 0 ? m_CurrentAnimation->FramesPerSec : 24.0f;//Animation FPS
    float TimeInTicks = dt * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, m_CurrentAnimation->NumberOfFrames); //Animation Duration

   // ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);//TODO

    Transforms.resize(mesh->m_BoneCount);

    for (uint32_t i = 0; i < mesh->m_BoneCount; i++) {
        Transforms[i] = mesh->m_BoneInfo[i].FinalTransformation;
    }
   
    return glm::mat4();
}

void Fracture::AnimatorComponent::Accept(ISceneProbe* visitor)
{
}
