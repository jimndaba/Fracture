#include "AnimationManager.h"
#include "Component/ComponentManager.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimationClip.h"
#include "AnimatorProbe.h"
#include "Component/AnimatorComponent.h"
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"

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

void Fracture::AnimationManager::AnimateTransform(float dt,const std::shared_ptr<AnimatorComponent>& m_animator, std::shared_ptr<TransformComponent>& transform)
{
    if (m_animator->m_CurrentAnimation)
    {
        float TicksPerSecond = m_animator->m_CurrentAnimation->FramesPerSec != 0 ? m_animator->m_CurrentAnimation->FramesPerSec : 24.0f;//Animation FPS
        float TimeInTicks = dt * TicksPerSecond;
        float AnimationTime = fmod(TimeInTicks, m_animator->m_CurrentAnimation->NumberOfFrames); //Animation Duration

        for (int i = 0; i < m_animator->m_CurrentAnimation->m_channels.size(); i++)
        {
            if (m_animator->m_CurrentAnimation->m_channels[i].Name == "Position")
            {
                glm::vec3 position;
                CalcInterpolatedvec3(position, m_animator->m_CurrentAnimation->m_channels[i].m_PositionKeys, AnimationTime);
                transform->setPosition(position);
            }
            if (m_animator->m_CurrentAnimation->m_channels[i].Name == "Scale")
            {
                glm::vec3 scale;
                CalcInterpolatedvec3(scale, m_animator->m_CurrentAnimation->m_channels[i].m_ScaleKeys, AnimationTime);
                transform->setScale(scale);
            }
            if (m_animator->m_CurrentAnimation->m_channels[i].Name == "Rotation")
            {
                glm::vec3 rotation;
                CalcInterpolatedvec3(rotation, m_animator->m_CurrentAnimation->m_channels[i].m_RotationKeys, AnimationTime);
                transform->setRotation(rotation);
            }
        }
    }
}

void Fracture::AnimationManager::AnimateRenderer(float dt,const std::shared_ptr<AnimatorComponent>& m_animator, std::shared_ptr<RenderComponent>& renderer)
{
    if (m_animator->m_CurrentAnimation)
    {
        float TicksPerSecond = m_animator->m_CurrentAnimation->FramesPerSec != 0 ? m_animator->m_CurrentAnimation->FramesPerSec : 24.0f;//Animation FPS
        float TimeInTicks = dt * TicksPerSecond;
        float AnimationTime = fmod(TimeInTicks, m_animator->m_CurrentAnimation->NumberOfFrames); //Animation Duration

        for (int i = 0; i < m_animator->m_CurrentAnimation->m_channels.size(); i++)
        {
            if (m_animator->m_CurrentAnimation->m_channels[i].Name == "Color")
            {
                glm::vec4 color;
                CalcInterpolatedvec4(color, m_animator->m_CurrentAnimation->m_channels[i].m_ColorKeys, AnimationTime);
                renderer->Color = color;
            }
        }
    }
}

void Fracture::AnimationManager::BoneTransformation(float dt, std::shared_ptr<AnimatorComponent>& m_animator, std::vector<glm::mat4>& Transforms)
{
    std::shared_ptr<Skeleton> skeleton = m_animator->m_skeleton;
   
    if (m_animator->m_CurrentAnimation)
    {
        float TicksPerSecond = m_animator->m_CurrentAnimation->FramesPerSec != 0 ? m_animator->m_CurrentAnimation->FramesPerSec : 24.0f;//Animation FPS
        float TimeInTicks = dt * TicksPerSecond;
        float AnimationTime = fmod(TimeInTicks, m_animator->m_CurrentAnimation->NumberOfFrames); //Animation Duration

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
                CalcInterpolatedScaling(Scaling, m_animator->m_CurrentAnimation->m_channels[bone.second], AnimationTime);

                // Interpolate rotation and generate rotation transformation matrix
                glm::quat RotationQ;
                CalcInterpolatedRotation(RotationQ, m_animator->m_CurrentAnimation->m_channels[bone.second], AnimationTime);


                // Interpolate translation and generate translation transformation matrix
                glm::vec3 Translation;
                CalcInterpolatedPosition(Translation, m_animator->m_CurrentAnimation->m_channels[bone.second], AnimationTime);

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


void Fracture::AnimationManager::CalcInterpolatedvec2(glm::vec2& out, const std::vector<AnimationKeyframe>& keyframes, const float& animationTime)
{
    // we need at least two values to interpolate...
    if (keyframes.size() == 1) {
        out = keyframes[0].VEC2;
        return;
    }
    uint32_t PositionIndex = FindNextKeyFrame(animationTime, keyframes);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < keyframes.size());

    float DeltaTime = keyframes[NextPositionIndex].Time - keyframes[PositionIndex].Time;
    float Factor = (animationTime - (float)keyframes[PositionIndex].Time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec2& StartPosition = keyframes[PositionIndex].VEC2;
    const glm::vec2& EndPosition = keyframes[NextPositionIndex].VEC2;
    glm::vec2 final = glm::lerp(StartPosition, EndPosition, Factor);
    out = glm::normalize(final);
}

void Fracture::AnimationManager::CalcInterpolatedvec3(glm::vec3& out, const std::vector<AnimationKeyframe>& keyframes, const float& animationTime)
{
    // we need at least two values to interpolate...
    if (keyframes.size() == 1) {
        out = keyframes[0].VEC3;
        return;
    }
    uint32_t PositionIndex = FindNextKeyFrame(animationTime, keyframes);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < keyframes.size());

    float DeltaTime = keyframes[NextPositionIndex].Time - keyframes[PositionIndex].Time;
    float Factor = (animationTime - (float)keyframes[PositionIndex].Time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& StartPosition = keyframes[PositionIndex].VEC3;
    const glm::vec3& EndPosition = keyframes[NextPositionIndex].VEC3;
    glm::vec3 final = glm::lerp(StartPosition, EndPosition, Factor);
    out = glm::normalize(final);
}

void Fracture::AnimationManager::CalcInterpolatedvec4(glm::vec4& out, const std::vector<AnimationKeyframe>& keyframes, const float& animationTime)
{
    // we need at least two values to interpolate...
    if (keyframes.size() == 1) {
        out = keyframes[0].VEC4;
        return;
    }
    uint32_t PositionIndex = FindNextKeyFrame(animationTime, keyframes);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < keyframes.size());

    float DeltaTime = keyframes[NextPositionIndex].Time - keyframes[PositionIndex].Time;
    float Factor = (animationTime - (float)keyframes[PositionIndex].Time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec4& StartPosition = keyframes[PositionIndex].VEC4;
    const glm::vec4& EndPosition = keyframes[NextPositionIndex].VEC4;
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

uint32_t Fracture::AnimationManager::FindNextKeyFrame(const float& time, const std::vector<AnimationKeyframe>& keyframes)
{
    assert(keyframes.size() > 0);
    for (int i = 0; i < keyframes.size() - 1; i++) {
        if (time < (float)keyframes[i + 1].Time) {
            return i;
        }
    }
    assert(0);
}

