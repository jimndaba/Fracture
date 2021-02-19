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
    ProfilerTimer timer("Animation Manager Update");
    const auto& components = ComponentManager::GetAllComponents<AnimatorComponent>();
    for (const auto& component : components)
    {
        if (component)
        {
            bool m_isPlaying = true;
            if(m_isPlaying)
            { 
                if (component->m_CurrentAnimation)
                {
                    float TicksPerSecond = component->m_CurrentAnimation->FramesPerSec != 0 ? component->m_CurrentAnimation->FramesPerSec : 24.0f;//Animation FPS
                    float TimeInTicks = dt * TicksPerSecond;
                    float AnimationTime = component->m_CurrentAnimation->AnimationTime;

                    AnimationTime += TimeInTicks;
                    if (AnimationTime >= component->m_CurrentAnimation->NumberOfFrames)
                    {
                        float duration = (float)component->m_CurrentAnimation->NumberOfFrames;
                        AnimationTime = modff(TimeInTicks, &duration);
                    }
                    component->m_CurrentAnimation->AnimationTime = AnimationTime;
                }

            }
          
            component->Accept(m_probe, dt);
        }
    }
}

void Fracture::AnimationManager::AnimateTransform(float dt,AnimatorComponent* m_animator, std::shared_ptr<TransformComponent>& transform)
{
    if (m_animator->m_CurrentAnimation)
    {      
        for (int i = 0; i < m_animator->m_CurrentAnimation->m_channels.size(); i++)
        {
            if (m_animator->m_CurrentAnimation->m_channels[i].Name == "Position")
            {
                glm::vec3 position;
                CalcInterpolatedvec3(position, m_animator->m_CurrentAnimation->m_channels[i].m_PositionKeys, m_animator->m_CurrentAnimation->AnimationTime);
                transform->setPosition(position);
            }
            if (m_animator->m_CurrentAnimation->m_channels[i].Name == "Scale")
            {
                glm::vec3 scale;
                CalcInterpolatedvec3(scale, m_animator->m_CurrentAnimation->m_channels[i].m_ScaleKeys, m_animator->m_CurrentAnimation->AnimationTime);
                transform->setScale(scale);
            }
            if (m_animator->m_CurrentAnimation->m_channels[i].Name == "Rotation")
            {
                glm::vec3 rotation;
                CalcInterpolatedvec3(rotation, m_animator->m_CurrentAnimation->m_channels[i].m_RotationKeys, m_animator->m_CurrentAnimation->AnimationTime);
                transform->setRotation(rotation);
            }
        }
    }
}

void Fracture::AnimationManager::AnimateRenderer(float dt,AnimatorComponent* m_animator, std::shared_ptr<RenderComponent>& renderer)
{
    if (m_animator->m_CurrentAnimation)
    {   
        for (int i = 0; i < m_animator->m_CurrentAnimation->m_channels.size(); i++)
        {
            if (m_animator->m_CurrentAnimation->m_channels[i].Name == "Color")
            {
                glm::vec4 color;
                CalcInterpolatedvec4(color, m_animator->m_CurrentAnimation->m_channels[i].m_ColorKeys, m_animator->m_CurrentAnimation->AnimationTime);
                renderer->Color = color;
            }
        }
    }
}

void Fracture::AnimationManager::BoneTransformation(const AnimationChannel& channel,const std::shared_ptr<Skeleton>& skeleton, const float& time)
{
    ProfilerTimer timer("Animation Manager Bone Transform"); 
    if (skeleton)
    {
        ProfilerTimer timer("Animation Channel");    

         // Interpolate translation and generate translation transformation matrix
        glm::mat4 m_translation = CalcInterpolatedPosition(channel, time);

        // Interpolate rotation and generate rotation transformation matrix
        glm::mat4 m_rotation = CalcInterpolatedRotation(channel, time);

        // Interpolate scaling and generate scaling transformation matrix
        glm::mat4 m_scale= CalcInterpolatedScaling(channel, time);
        
        //glm::mat4 m_LocalTransform = m_translation * m_rotation * m_scale;
        if (skeleton->m_BoneMapping.find(channel.Name) != skeleton->m_BoneMapping.end())
        {
            uint32_t BoneIndex = skeleton->m_BoneMapping[channel.Name];
            skeleton->m_BoneInfo[BoneIndex].LocalTransformation = m_translation * m_rotation * m_scale;
        }
    }
}

void Fracture::AnimationManager::UpdateFinalTransforms(const std::shared_ptr<Skeleton>& skeleton)
{  
    UpdateHierachy(skeleton->m_Root,skeleton, skeleton->m_Root->LocalTransformation);
}

void Fracture::AnimationManager::UpdateHierachy(const std::shared_ptr<Bone>& bone, const std::shared_ptr<Skeleton>& skeleton,const glm::mat4& parentTransform)
{
    auto mapInfo = skeleton->m_BoneMapping;
    if (mapInfo.find(skeleton->m_BoneInfo[bone->ID].Name) != mapInfo.end())
    {       
        bone->LocalTransformation = skeleton->m_BoneInfo[bone->ID].LocalTransformation;
        glm::mat4 GlobalTransformation = parentTransform * bone->LocalTransformation;
        skeleton->m_BoneInfo[bone->ID].FinalTransformation = GlobalTransformation * skeleton->m_BoneInfo[bone->ID].BoneOffset;;// 
        for (int i = 0; i < bone->m_Children.size(); i++)
        {
            UpdateHierachy(bone->m_Children[i],skeleton,GlobalTransformation);
        }
    }    
}

void Fracture::AnimationManager::PushTransforms(AnimatorComponent* animator)
{
    ProfilerTimer timer("Push Transforms");
    animator->pushTransform();
}

float Fracture::AnimationManager::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

glm::mat4 Fracture::AnimationManager::CalcInterpolatedScaling(const AnimationChannel& channel, const float& animationTime)
{
    ProfilerTimer timer("Interpolate Scale");
    // we need at least two values to interpolate...
    if (channel.m_ScaleKeys.size() == 1) {
        return glm::scale(glm::mat4(1.0f), channel.m_ScaleKeys[0].Scale_key);
    }

    uint32_t ScaleIndex = FindScale(animationTime, channel);
    uint32_t NextScaleIndex = (ScaleIndex + 1);
    assert(NextScaleIndex < channel.m_ScaleKeys.size());
    float Factor = GetScaleFactor((float)channel.m_ScaleKeys[ScaleIndex].Time,(float)channel.m_ScaleKeys[NextScaleIndex].Time,animationTime);  
    const glm::vec3& StartPosition = channel.m_ScaleKeys[ScaleIndex].Scale_key;
    const glm::vec3& EndPosition = channel.m_ScaleKeys[NextScaleIndex].Scale_key;
   
    glm::vec3 final = glm::mix(StartPosition, EndPosition, Factor);
    return glm::scale(glm::mat4(1.0f),final);
}

glm::mat4 Fracture::AnimationManager::CalcInterpolatedRotation(const AnimationChannel& channel, const float& animationTime)
{
    ProfilerTimer timer("Interpolate Rotation");
    // we need at least two values to interpolate...
    if (channel.m_RotationKeys.size() == 1) {
        return glm::toMat4(channel.m_RotationKeys[0].Rotation_key);
    }

    uint32_t RotationIndex = FindRotation(animationTime, channel);
    uint32_t NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < channel.m_RotationKeys.size());
    
    float Factor = GetScaleFactor((float)channel.m_RotationKeys[RotationIndex].Time, (float)channel.m_RotationKeys[NextRotationIndex].Time, animationTime);  
    const glm::quat& StartRotationQ = channel.m_RotationKeys[RotationIndex].Rotation_key;
    const glm::quat& EndRotationQ = channel.m_RotationKeys[NextRotationIndex].Rotation_key;
    glm::quat final = glm::mix(StartRotationQ, EndRotationQ, Factor);
    final = glm::normalize(final);
    return glm::toMat4(final);
}

glm::mat4 Fracture::AnimationManager::CalcInterpolatedPosition(const AnimationChannel& channel, const float& animationTime)
{
    ProfilerTimer timer("Interpolate Position");
    // we need at least two values to interpolate...
    if (channel.m_PositionKeys.size() == 1) {
       return glm::translate(channel.m_PositionKeys[0].Position_key);
    }
    uint32_t PositionIndex = FindPosition(animationTime, channel);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    
    assert(NextPositionIndex < channel.m_PositionKeys.size());
    float DeltaTime = (float)channel.m_PositionKeys[NextPositionIndex].Time - (float)channel.m_PositionKeys[PositionIndex].Time;
    float Factor = GetScaleFactor((float)channel.m_PositionKeys[PositionIndex].Time, (float)channel.m_PositionKeys[NextPositionIndex].Time, animationTime);  
    const glm::vec3& StartPosition = channel.m_PositionKeys[PositionIndex].Position_key;
    const glm::vec3& EndPosition = channel.m_PositionKeys[NextPositionIndex].Position_key;
    //glm::vec3 final =glm::mix(StartPosition, EndPosition, Factor);
    glm::vec3 final = glm::lerp(StartPosition, EndPosition, Factor);
    return  glm::translate(glm::mat4(1.0f),final);
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

    float DeltaTime = (float)keyframes[NextPositionIndex].Time - (float)keyframes[PositionIndex].Time;
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

    float DeltaTime = (float)keyframes[NextPositionIndex].Time - (float)keyframes[PositionIndex].Time;
    float Factor = (animationTime - (float)keyframes[PositionIndex].Time) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& StartPosition = keyframes[PositionIndex].VEC3;
    const glm::vec3& EndPosition = keyframes[NextPositionIndex].VEC3;
    glm::vec3 final = glm::lerp(StartPosition, EndPosition, Factor);
    out = final;
   // out = glm::normalize(final);
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

    float DeltaTime = (float)keyframes[NextPositionIndex].Time - (float)keyframes[PositionIndex].Time;
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
    return -1;
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
    return -1;
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
    return -1;
}

uint32_t Fracture::AnimationManager::FindNextKeyFrame(const float& time, const std::vector<AnimationKeyframe>& keyframes)
{
    assert(keyframes.size() > 0);
    for (int i = 0; i < keyframes.size() - 1 ; i++) {
        if (time < (float)keyframes[i + 1].Time) {
            return i;
        }
    }
    assert(0);
    return -1;
}

