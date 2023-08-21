#include "FracturePCH.h"
#include "AnimationTasks.h"
#include "AnimationSystem.h"
#include "Assets/AssetManager.h"
#include "Animation/GraphNodes/PoseNodes.h"
Fracture::SampleTask::SampleTask():
	IAnimationTask()
{
}

void Fracture::SampleTask::Execute(AnimationContext& context)
{
	const auto& clip = AssetManager::GetAnimationByID(ClipID);
	const auto& mesh = AssetManager::GetStaticByIDMesh(context.MeshID);
	float AnimationTime = 0;
	if (clip)
	{
		auto& Pose = context._system->mPool->GetPoseBuffer()->Pose;

		AnimationTime = Time;
		AnimationTime += clip->FramesPerSec * context.Time;
		AnimationTime = fmod(AnimationTime, clip->Duration);
		
		const auto& node = std::static_pointer_cast<AnimationPoseNode>(context._graph->GetNode(NodeID));
		node->Time = AnimationTime;

		context._system->SampleAnimation(mesh.get(), Pose, *clip.get(), AnimationTime);
	}
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
	context._system->Blend(context.EntityID, BlendFunction, BlendSpace,in_pose1,in_pose2,factor);
}
