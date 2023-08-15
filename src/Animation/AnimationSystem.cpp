#include "FracturePCH.h"
#include "AnimationSystem.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "Assets/AssetManager.h"
#include "Animation/AnimationClip.h"
#include "Rendering/Mesh.h"

std::unique_ptr<Fracture::AnimationSystem> Fracture::AnimationSystem::mInstance;

Fracture::AnimationSystem::AnimationSystem()
{
}

void Fracture::AnimationSystem::Init()
{
}

void Fracture::AnimationSystem::Update(float dt)
{
    mGlobalPoses.clear();

    const auto& components = SceneManager::GetAllComponents<AnimationComponent>();
    for (const auto& component : components)
    {
        if (!component->HasAnimationSet)
            continue;

        const auto& current_animation = AssetManager::GetAnimationByID(component->CurrentAnimation);
        if (component && current_animation)
        {
            bool m_isPlaying = true;
            float AnimationTime = 0;
            
            if (m_isPlaying)
            {
                AnimationTime = component->AnimationTime;
                AnimationTime += current_animation->FramesPerSec * dt;
                AnimationTime = fmod(AnimationTime, current_animation->Duration);
                component->AnimationTime = AnimationTime;
                current_animation->AnimationTime = AnimationTime;
            }

            UpdatePose(component->GetID(), current_animation.get(),component->Play);
        }
    }
}

void Fracture::AnimationSystem::UpdatePose(Fracture::UUID entity, AnimationClip* clip, bool playing)
{
    const auto& mesh_component = SceneManager::GetComponent<MeshComponent>(entity);
    auto mesh = AssetManager::GetStaticByIDMesh(mesh_component->Mesh);
    
    mGlobalPoses[entity] = GlobalPose{};
    mGlobalPoses[entity].GlobalPoses.resize(mesh->mBones.size());
    mGlobalPoses[entity].LocalPoses.resize(mesh->mBones.size());
    mGlobalPoses[entity].FinalPoses.resize(mesh->mBones.size());
       
    if (playing)
    {
        for (int i = 0; i < mesh->mBones.size(); i++)
        {
            auto& bone =  mesh->mBones[mesh->mBoneOrder[i]];
            glm::mat4 parent_transform = glm::mat4(1.0f);

            if (i > 0) {
                parent_transform = mGlobalPoses[entity].LocalPoses[bone.ParentID];
            }

            AnimationTrack track;
            if (!GetBoneTrack(clip, bone.Name, track))
                continue;
                       
            bone.LocalTransformation = BoneTransformation(track, clip->AnimationTime);
            mGlobalPoses[entity].LocalPoses[bone.ID] = parent_transform *  bone.LocalTransformation;      
            mGlobalPoses[entity].GlobalPoses[bone.ID] = mGlobalPoses[entity].LocalPoses[bone.ID] * mesh->mBones[bone.ID].InverseBindTransform;
        }      
    }
    else
    {
        for (int i = 0; i < mesh->mBones.size(); i++)
        {
            auto& bone = mesh->mBones[mesh->mBoneOrder[i]];
            glm::mat4 parent_transform = glm::mat4(1.0f);

            if (i > 0) {
                parent_transform = mGlobalPoses[entity].LocalPoses[bone.ParentID];
            }

            mGlobalPoses[entity].LocalPoses[bone.ID] = bone.BindTransformation;
            mGlobalPoses[entity].GlobalPoses[bone.ID] = mGlobalPoses[entity].LocalPoses[bone.ID] * mesh->mBones[bone.ID].InverseBindTransform;
        }
    }
}

bool Fracture::AnimationSystem::GetBoneTrack(AnimationClip* clip, const std::string& name, AnimationTrack& outTrack)
{
    for (uint32_t i = 0; i < clip->AnimationTracks.size(); i++)
    {
        auto& anim = clip->AnimationTracks[i];
        if (anim.Name == name)
        {
            outTrack = clip->AnimationTracks[i];
            return true;
        }
    }
    return false;
}

bool Fracture::AnimationSystem::HasGlobalPose(UUID entity)
{
    return mGlobalPoses.find(entity) != mGlobalPoses.end();
}

Fracture::AnimationSystem* Fracture::AnimationSystem::Instance()
{
    if (!mInstance)
        mInstance = std::make_unique<AnimationSystem>();
    return mInstance.get();
}


glm::mat4 Fracture::AnimationSystem::CalcInterpolatedScaling(AnimationTrack& outTrack, const float& animationTime)
{
    if (outTrack.Scales.size() == 1) {
        return  glm::scale(glm::mat4(1.0f), outTrack.Scales[0].Scale);
    }
    uint32_t PositionIndex = FindNextScale(animationTime, outTrack);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < channel.m_ScaleKeys.size());

    float Factor = GetScaleFactor((float)outTrack.Scales[PositionIndex].Time, (float)outTrack.Scales[NextPositionIndex].Time, animationTime);
    Factor = glm::clamp(Factor, 0.0f, 1.0f);
    const glm::vec3& StartPosition = outTrack.Scales[PositionIndex].Scale;
    const glm::vec3& EndPosition = outTrack.Scales[NextPositionIndex].Scale;
    glm::vec3 final = glm::mix(StartPosition, EndPosition, Factor);

    return glm::scale(glm::mat4(1.0f), final);
}

glm::mat4 Fracture::AnimationSystem::CalcInterpolatedRotation(AnimationTrack& outTrack, const float& animationTime)
{   
    if (outTrack.mNumRotationKeys == 1) {
        auto rotation = glm::normalize(outTrack.Rotations[0].Rotation);
        return glm::toMat4(rotation);
    }

    uint32_t RotationIndex = FindNextRotation(animationTime, outTrack);
    uint32_t NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < channel.mNumRotationKeys);
    float Factor = GetScaleFactor(outTrack.Rotations[RotationIndex].Time, outTrack.Rotations[NextRotationIndex].Time, animationTime);
    glm::quat StartRotationQ = outTrack.Rotations[RotationIndex].Rotation;
    glm::quat EndRotationQ = outTrack.Rotations[NextRotationIndex].Rotation;
    glm::quat final = glm::slerp(StartRotationQ, EndRotationQ, Factor);
    final = glm::normalize(final);
    return glm::toMat4(final);
}

glm::mat4 Fracture::AnimationSystem::CalcInterpolatedPosition(AnimationTrack& outTrack, const float& animationTime)
{
    if (outTrack.mNumPositionKeys == 1) {
        return glm::translate(glm::mat4(1.0f), outTrack.Positions[0].Position);
    }

    uint32_t PositionIndex = FindNextPosition(animationTime, outTrack);
    uint32_t NextPositionIndex = (PositionIndex + 1);

    assert(NextPositionIndex < outTrack.mNumPositionKeys);
    float Factor = GetScaleFactor((float)outTrack.Positions[PositionIndex].Time, (float)outTrack.Positions[NextPositionIndex].Time, animationTime);
    const glm::vec3& StartPosition = outTrack.Positions[PositionIndex].Position;
    const glm::vec3& EndPosition = outTrack.Positions[NextPositionIndex].Position;
    glm::vec3 final = glm::mix(StartPosition, EndPosition, Factor);
    return  glm::translate(glm::mat4(1.0f), final);
}

glm::mat4 Fracture::AnimationSystem::BoneTransformation(AnimationTrack& outTrack,float time)
{    
    glm::mat4 m_translation = CalcInterpolatedPosition(outTrack, time);
    glm::mat4 m_rotation = CalcInterpolatedRotation(outTrack, time);
    glm::mat4 m_scale = CalcInterpolatedScaling(outTrack, time);
    return m_translation * m_rotation * m_scale;
}

float Fracture::AnimationSystem::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
{
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;
    scaleFactor = midWayLength / framesDiff;
    return scaleFactor;
}

uint32_t Fracture::AnimationSystem::FindNextScale(const float& time, AnimationTrack& outTrack)
{
    assert(outTrack.mNumScaleKeys > 0);
    for (int i = 0; i < outTrack.mNumScaleKeys - 1; i++) {
        if (time < (float)outTrack.Scales[i + 1].Time) {
            return i;
        }
    }
    assert(0);
}
uint32_t Fracture::AnimationSystem::FindNextRotation(const float& time, AnimationTrack& outTrack)
{
    assert(outTrack.mNumRotationKeys > 0);
    for (int i = 0; i < outTrack.mNumRotationKeys - 1; i++) {
        if (time < (float)outTrack.Rotations[i + 1].Time) {
            return i;
        }
    }
    assert(0);
}
uint32_t Fracture::AnimationSystem::FindNextPosition(const float& time, AnimationTrack& outTrack)
{
    assert(outTrack.mNumPositionKeys > 0);
    for (int i = 0; i < outTrack.mNumPositionKeys - 1; i++) {
        if (time < (float)outTrack.Positions[i + 1].Time) {
            return i;
        }
    }
    assert(0);
}

