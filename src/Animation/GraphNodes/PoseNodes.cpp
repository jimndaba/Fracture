#include "FracturePCH.h"
#include "PoseNodes.h"
#include "Animation/AnimationGraph.h"
#include "Animation/AnimationTasks.h"

Fracture::ReferencePoseNode::ReferencePoseNode() : IPoseNode()
{
	Name = "Reference Pose";
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Pose",
			.PinType = PinValueType::Pose
			}
	};

}

void Fracture::ReferencePoseNode::Process(AnimationContext& context)
{
	if (AnimationSet)
	{
		std::shared_ptr<SampleTask> task = std::make_shared<SampleTask>();
		task->Time = 0;
		task->ClipID = AnimationClipID;
		task->NodeID = NodeID;
		context._graph->PushTask(task);
	}
}

Fracture::AnimationPoseNode::AnimationPoseNode() : IPoseNode()
{
	Name = "Animation Pose";
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Pose",
			.PinType = PinValueType::Pose
			}
	};
}

void Fracture::AnimationPoseNode::Process(AnimationContext& context)
{
	float AnimationTime = 0.0f;
	if (AnimationSet)
	{
		PoseNodeID = this->NodeID;

		std::shared_ptr<SampleTask> task = std::make_shared<SampleTask>();
		task->Time = Time += context.Time;
		task->ClipID = AnimationClipID;
		task->NodeID = NodeID;	
		context._graph->PushTask(task);
	
	}
}
