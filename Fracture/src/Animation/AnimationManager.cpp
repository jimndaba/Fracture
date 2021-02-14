#include "AnimationManager.h"
#include "Component/ComponentManager.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimationClip.h"
#include "AnimatorProbe.h"
#include "Component/AnimatorComponent.h"

Fracture::AnimationManager::AnimationManager()
{
    m_probe = new AnimatorProbe(*this);
}

Fracture::AnimationManager::~AnimationManager()
{
    delete m_probe;
}

void Fracture::AnimationManager::OnUpdate(float dt)
{
    for (auto component : ComponentManager::GetAllComponents<AnimatorComponent>())
    {
        component->Accept(m_probe);
    }
}

void Fracture::AnimationManager::BoneTransformation(float dt, std::shared_ptr<AnimatorComponent>& m_animtor, std::vector<glm::mat4>& Transforms)
{
    std::shared_ptr<AnimatorComponent> m_animtor;
    std::shared_ptr<Skeleton> skeleton = m_animtor->m_skeleton;
   
    if (m_animtor->m_CurrentAnimation)
    {
        float TicksPerSecond = m_animtor->m_CurrentAnimation->FramesPerSec != 0 ? m_animtor->m_CurrentAnimation->FramesPerSec : 24.0f;//Animation FPS
        float TimeInTicks = dt * TicksPerSecond;
        float AnimationTime = fmod(TimeInTicks, m_animtor->m_CurrentAnimation->NumberOfFrames); //Animation Duration

        if (skeleton)
        {
            for (auto bone : skeleton->m_BoneMapping)
            {
                std::string NodeName = bone.first;
                glm::mat4 ParentTransform;

                int parentIndex = skeleton->BoneParents[skeleton->m_BoneMapping[NodeName]];

                if (parentIndex == -1)
                {
                    ParentTransform = glm::mat4();
                }
                else
                {
                    ParentTransform = skeleton->m_BoneInfo[parentIndex].FinalTransformation;
                }
                glm::mat4 m_LocalTransform = glm::mat4();

                // Interpolate scaling and generate scaling transformation matrix
                glm::vec3 Scaling;
                CalcInterpolatedScaling(Scaling, m_animtor->m_CurrentAnimation->m_channels[bone.second], AnimationTime);

                // Interpolate rotation and generate rotation transformation matrix
                glm::quat RotationQ;
                CalcInterpolatedRotation(RotationQ, m_animtor->m_CurrentAnimation->m_channels[bone.second], AnimationTime);


                // Interpolate translation and generate translation transformation matrix
                glm::vec3 Translation;
                CalcInterpolatedPosition(Translation, m_animtor->m_CurrentAnimation->m_channels[bone.second], AnimationTime);

                // Combine the above transformations        
                glm::mat4 m_rotation, m_translation, m_scale;

                m_translation = glm::translate(Translation);
                m_scale = glm::scale(Scaling);
                m_rotation = glm::toMat4(RotationQ);
                m_LocalTransform = m_translation * m_rotation * m_scale;

                glm::mat4 GlobalTransformation = ParentTransform * m_LocalTransform;

                if (skeleton->m_BoneMapping.find(NodeName) != skeleton->m_BoneMapping.end()) {
                    uint32_t BoneIndex = skeleton->m_BoneMapping[NodeName];
                    skeleton->m_BoneInfo[BoneIndex].FinalTransformation = GlobalTransformation * GlobalTransformation * skeleton->m_BoneInfo[BoneIndex].BoneOffset;
                }
            }

            Transforms.resize(skeleton->NumBones);
            for (uint32_t i = 0; i < skeleton->NumBones; i++) {
                Transforms[i] = skeleton->m_BoneInfo[i].FinalTransformation;
            }
        }


    }
}

void Fracture::AnimationManager::CalcInterpolatedScaling(glm::vec3& out, const AnimationChannel& channel, const float& animationTime)
{

    // we need at least two values to interpolate...
    if (channel.m_ScaleKeys.size() == 1) {
        out = channel.m_ScaleKeys[0].Scale_key;
        return;
    }

    uint32_t ScaleIndex = FindRotation(animationTime, channel);
    uint32_t NextScaleIndex = (ScaleIndex + 1);
    assert(NextScaleIndex < channel.m_ScaleKeys.size());

    float DeltaTime = channel.m_ScaleKeys[NextScaleIndex].Time - channel.m_ScaleKeys[ScaleIndex].Time;
    float Factor = (animationTime - (float)channel.m_ScaleKeys[ScaleIndex].Time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& StartPosition = channel.m_ScaleKeys[ScaleIndex].Scale_key;
    const glm::vec3& EndPosition = channel.m_ScaleKeys[NextScaleIndex].Scale_key;
    glm::vec3 final = glm::lerp(StartPosition, EndPosition, Factor);
    out = glm::normalize(final);
}

void Fracture::AnimationManager::CalcInterpolatedRotation(glm::quat& out, const AnimationChannel& channel, const float& animationTime)
{
    // we need at least two values to interpolate...
    if (channel.m_RotationKeys.size() == 1) {
        out = channel.m_RotationKeys[0].Rotation_key;
        return;
    }

    uint32_t RotationIndex = FindRotation(animationTime, channel);
    uint32_t NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < channel.m_RotationKeys.size());

    float DeltaTime = channel.m_RotationKeys[NextRotationIndex].Time - channel.m_RotationKeys[RotationIndex].Time;
    float Factor = (animationTime - (float)channel.m_RotationKeys[RotationIndex].Time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::quat& StartRotationQ = channel.m_RotationKeys[RotationIndex].Rotation_key;
    const glm::quat& EndRotationQ = channel.m_RotationKeys[NextRotationIndex].Rotation_key;
    glm::quat final = glm::mix(StartRotationQ, EndRotationQ, Factor);
    out = glm::normalize(final);
}

void Fracture::AnimationManager::CalcInterpolatedPosition(glm::vec3& out, const AnimationChannel& channel, const float& animationTime)
{
    // we need at least two values to interpolate...
    if (channel.m_PositionKeys.size() == 1) {
        out = channel.m_PositionKeys[0].Position_key;
        return;
    }
    uint32_t PositionIndex = FindRotation(animationTime, channel);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < channel.m_PositionKeys.size());

    float DeltaTime = channel.m_PositionKeys[NextPositionIndex].Time - channel.m_PositionKeys[PositionIndex].Time;
    float Factor = (animationTime - (float)channel.m_PositionKeys[PositionIndex].Time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& StartPosition = channel.m_PositionKeys[PositionIndex].Position_key;
    const glm::vec3& EndPosition = channel.m_PositionKeys[NextPositionIndex].Position_key;
    glm::vec3 final = glm::lerp(StartPosition, EndPosition, Factor);
    out = glm::normalize(final);
}

void Fracture::AnimationManager::CalcInterpolatedColor(glm::vec4& out, const AnimationChannel& channel, const float& animationTime)
{
    // we need at least two values to interpolate...
    if (channel.m_ColorKeys.size() == 1) {
        out = channel.m_ColorKeys[0].Color_key;
        return;
    }
    uint32_t ColorIndex = FindRotation(animationTime, channel);
    uint32_t NextColorIndex = (ColorIndex + 1);
    assert(NextColorIndex < channel.m_ColorKeys.size());

    float DeltaTime = channel.m_ColorKeys[NextColorIndex].Time - channel.m_ColorKeys[ColorIndex].Time;
    float Factor = (animationTime - (float)channel.m_ColorKeys[ColorIndex].Time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec4& StartPosition = channel.m_ColorKeys[ColorIndex].Color_key;
    const glm::vec4& EndPosition = channel.m_ColorKeys[NextColorIndex].Color_key;
    glm::vec4 final = glm::lerp(StartPosition, EndPosition, Factor);
    out = glm::normalize(final);
}

uint32_t Fracture::AnimationManager::FindRotation(const float& time, const AnimationChannel& channel)
{
    assert(channel.m_RotationKeys.size() > 0);
    for (int i = 0; i < channel.m_RotationKeys.size() - 1; i++) {
        if (time < (float)channel.m_RotationKeys[i + 1].Time) {
            return i;
        }
    }
    assert(0);
}

uint32_t Fracture::AnimationManager::FindScale(const float& time, const AnimationChannel& channel)
{
    assert(channel.m_ScaleKeys.size() > 0);
    for (int i = 0; i < channel.m_ScaleKeys.size() - 1; i++) {
        if (time < (float)channel.m_ScaleKeys[i + 1].Time) {
            return i;
        }
    }
    assert(0);
}

uint32_t Fracture::AnimationManager::FindPosition(const float& time, const AnimationChannel& channel)
{
    assert(channel.m_PositionKeys.size() > 0);
    for (int i = 0; i < channel.m_PositionKeys.size() - 1; i++) {
        if (time < (float)channel.m_PositionKeys[i + 1].Time) {
            return i;
        }
    }
    assert(0);
}

uint32_t Fracture::AnimationManager::FindColor(const float& time, const AnimationChannel& channel)
{
    assert(channel.m_ColorKeys.size() > 0);
    for (int i = 0; i < channel.m_ColorKeys.size() - 1; i++) {
        if (time < (float)channel.m_ColorKeys[i + 1].Time) {
            return i;
        }
    }
    assert(0);
}
