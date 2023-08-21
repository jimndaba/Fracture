#include "FracturePCH.h"
#include "AnimationSystem.h"
#include "World/Components.h"
#include "World/SceneManager.h"
#include "Assets/AssetManager.h"
#include "Animation/AnimationClip.h"
#include "AnimationState.h"
#include "Rendering/Mesh.h"

std::unique_ptr<Fracture::AnimationSystem> Fracture::AnimationSystem::mInstance;

Fracture::AnimationSystem::AnimationSystem()
{
}

void Fracture::AnimationSystem::Init()
{
    mPool = std::make_unique<PoseBufferPool>(8);
    mPool->Init();
}

void Fracture::AnimationSystem::Update(float dt)
{
    OPTICK_EVENT(); 
    if (IsPlaying)
    {
        for (const auto& graph : mGraphs)
        {
            mPool->ReleaseAllBuffers();
            graph.second->OnUpdate(dt);
        }
    }
}

void Fracture::AnimationSystem::UpdatePose(Fracture::UUID entity, AnimationClip* clip, float time)
{
    /*
    OPTICK_EVENT();
    const auto& mesh_component = SceneManager::GetComponent<MeshComponent>(entity);
    auto mesh = AssetManager::GetStaticByIDMesh(mesh_component->Mesh);
    
    mGlobalPoses[entity] = GlobalPose{};
    mGlobalPoses[entity].GlobalPoses.resize(mesh->mBones.size());
    mGlobalPoses[entity].LocalPoses.resize(mesh->mBones.size());
       
    for (int i = 0; i < mesh->mBones.size(); i++)
    {
        auto& bone = mesh->mBones[mesh->mBoneOrder[i]];
        glm::mat4 parent_transform = glm::mat4(1.0f);

        if (i > 0) {
            parent_transform = mGlobalPoses[entity].LocalPoses[bone.ParentID];
        }

        AnimationTrack track;
        if (!GetBoneTrack(clip, bone.Name, track))
            continue;

        bone.LocalTransformation = BoneTransformation(track, time);
        mGlobalPoses[entity].LocalPoses[bone.ID] = parent_transform * bone.LocalTransformation;
        mGlobalPoses[entity].GlobalPoses[bone.ID] = mGlobalPoses[entity].LocalPoses[bone.ID] * mesh->mBones[bone.ID].InverseBindTransform;
    }
    */
}

void Fracture::AnimationSystem::Blend(Fracture::UUID in_graph, BlendFunctionType func, BlendSpaceType space, std::vector<PoseSample> inPose1, std::vector<PoseSample> inPose2, float factor)
{
    const auto& graph = mGraphs[in_graph];

   
    auto buffer1 = inPose1;
    auto buffer2 = inPose2;

    auto& new_buffer = mPool->GetPoseBuffer(0)->Pose; //graph->GetPoseBufferAndSwap();
    mPool->ReleasePoseBuffer(1);

    new_buffer.clear();
    new_buffer.resize(inPose1.size());
    for (int i = 0; i < inPose1.size(); i++)
    {

        new_buffer[i] = mBlender.AdditiveBlend1D(buffer1[i], buffer2[i], factor);
       

    }
    int lastBuffer = mPool->GetLastBufferInUse();
    if(lastBuffer > 0)
        mPool->MovePoseAndClearBuffer(lastBuffer, 1);
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
    return mGraphs.find(entity) != mGraphs.end();
}

bool Fracture::AnimationSystem::EntityHasGraph(UUID entity)
{
    return mGraphs.find(entity) != mGraphs.end();
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

glm::vec3 Fracture::AnimationSystem::SampleScaling(AnimationTrack& outTrack, const float& animationTime)
{
    if (outTrack.Scales.size() == 1) {
        return outTrack.Scales[0].Scale;
    }
    uint32_t PositionIndex = FindNextScale(animationTime, outTrack);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < channel.m_ScaleKeys.size());

    float Factor = GetScaleFactor((float)outTrack.Scales[PositionIndex].Time, (float)outTrack.Scales[NextPositionIndex].Time, animationTime);
    Factor = glm::clamp(Factor, 0.0f, 1.0f);
    const glm::vec3& StartPosition = outTrack.Scales[PositionIndex].Scale;
    const glm::vec3& EndPosition = outTrack.Scales[NextPositionIndex].Scale;
    glm::vec3 final = glm::mix(StartPosition, EndPosition, Factor);

    return final;
}

glm::quat Fracture::AnimationSystem::SampleRotation(AnimationTrack& outTrack, const float& animationTime)
{
    if (outTrack.mNumRotationKeys == 1) {
        auto rotation = glm::normalize(outTrack.Rotations[0].Rotation);
        return rotation;
    }

    uint32_t RotationIndex = FindNextRotation(animationTime, outTrack);
    uint32_t NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < channel.mNumRotationKeys);
    float Factor = GetScaleFactor(outTrack.Rotations[RotationIndex].Time, outTrack.Rotations[NextRotationIndex].Time, animationTime);
    glm::quat StartRotationQ = outTrack.Rotations[RotationIndex].Rotation;
    glm::quat EndRotationQ = outTrack.Rotations[NextRotationIndex].Rotation;
    glm::quat final = glm::slerp(StartRotationQ, EndRotationQ, Factor);
    final = glm::normalize(final);
    return final;
}

glm::vec3 Fracture::AnimationSystem::SamplePosition(AnimationTrack& outTrack, const float& animationTime)
{
    if (outTrack.mNumPositionKeys == 1) {
        return outTrack.Positions[0].Position;
    }

    uint32_t PositionIndex = FindNextPosition(animationTime, outTrack);
    uint32_t NextPositionIndex = (PositionIndex + 1);

    assert(NextPositionIndex < outTrack.mNumPositionKeys);
    float Factor = GetScaleFactor((float)outTrack.Positions[PositionIndex].Time, (float)outTrack.Positions[NextPositionIndex].Time, animationTime);
    const glm::vec3& StartPosition = outTrack.Positions[PositionIndex].Position;
    const glm::vec3& EndPosition = outTrack.Positions[NextPositionIndex].Position;
    glm::vec3 final = glm::mix(StartPosition, EndPosition, Factor);
    return  final;
}

glm::mat4 Fracture::AnimationSystem::BoneTransformation(AnimationTrack& outTrack,float time)
{    
    glm::mat4 m_translation = CalcInterpolatedPosition(outTrack, time);
    glm::mat4 m_rotation = CalcInterpolatedRotation(outTrack, time);
    glm::mat4 m_scale = CalcInterpolatedScaling(outTrack, time);
    return m_translation * m_rotation * m_scale;
}

void Fracture::AnimationSystem::SampleAnimation(const StaticMesh* mesh,std::vector<PoseSample>& outSample, AnimationClip& clip, float time)
{
    outSample.clear();
    outSample.resize(mesh->mBones.size());
    for (int i = 0; i < mesh->mBones.size(); i++)
    {
        auto& bone = mesh->mBones[mesh->mBoneOrder[i]];       

        AnimationTrack track;
        if (!GetBoneTrack(&clip, bone.Name, track))
            continue;

        PoseSample sample;
        sample.Position = SamplePosition(track, time);
        sample.Scale = SampleScaling(track, time);
        sample.Rotation = SampleRotation(track, time);
        outSample[i]= sample;
    }
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

void Fracture::AnimationSystem::ReloadGraphForAllEntities(UUID graph)
{
    if (mEntityToGraphTracker.find(graph) != mEntityToGraphTracker.end())
    {
        for (const auto& entiy : mEntityToGraphTracker[graph])
        {
            if (mGraphs.find(entiy) != mGraphs.end())
            {
                mGraphs.erase(entiy);
                mGraphs[entiy] = AssetManager::GetAnimationGraphByID(graph);
            }
        }

    }
}

