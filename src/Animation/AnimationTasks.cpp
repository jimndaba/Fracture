#include "FracturePCH.h"
#include "AnimationTasks.h"
#include "AnimationSystem.h"
#include "Assets/AssetManager.h"
#include "Animation/GraphNodes/PoseNodes.h"
#include "World/SceneManager.h"
Fracture::SampleTask::SampleTask():
	IAnimationTask()
{
}

void Fracture::SampleTask::Execute(AnimationContext& context)
{
	const auto& mesh = AssetManager::GetStaticByIDMesh(context.MeshID);
	auto& Pose = context._system->mPool->GetPoseBuffer()->Pose;
	const auto& node = (AnimationPoseNode*)(context._graph->GetNode(NodeID));
	const auto& anim_component = SceneManager::GetComponent<AnimationComponent>(context.EntityID);
	const auto& clip = AssetManager::GetAnimationByID(node->Result.AnimationClip);
	if (!node)
		return;

	anim_component->AnimationTracks[node->Result.AnimationClip].Duration = clip->Duration;
	anim_component->AnimationTracks[node->Result.AnimationClip].FrameTime += clip->FramesPerSec * context.Time;
	anim_component->AnimationTracks[node->Result.AnimationClip].FrameTime = fmod(anim_component->AnimationTracks[node->Result.AnimationClip].FrameTime, clip->Duration);

	float AnimationTime = anim_component->AnimationTracks[node->Result.AnimationClip].FrameTime;
	

	context._system->SampleAnimation(mesh.get(), Pose, node->Result.AnimationClip, AnimationTime,context.Time);
}

Fracture::BlendTask::BlendTask() :
	IAnimationTask()
{
}

void Fracture::BlendTask::Execute(AnimationContext& context)
{
	auto in_pose1 = context._system->mPool->GetPoseBuffer(0)->Pose;
	auto in_pose2 = context._system->mPool->GetPoseBuffer(1)->Pose;

	if (in_pose1.empty() || in_pose2.empty())
	{
		return;
	}
	context._system->Blend(context.GraphID, BlendFunction, BlendSpace,in_pose1,in_pose2,factor);
}
