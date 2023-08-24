#include "FracturePCH.h"
#include "PoseNodes.h"
#include "Animation/AnimationGraph.h"
#include "Animation/AnimationTasks.h"
#include "AnimationClipNode.h"

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
	InputPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "AnimationClip",
			.PinType = PinValueType::Pose,
			}		
	};
}

void Fracture::AnimationPoseNode::Process(AnimationContext& context)
{
	const auto& node = (IPoseNode*)context._graph->GetNode(InputPins[0].NodeID);
	if (node)
	{		
		HasAnimation = true;
		CurrentAnimation = node->Result.AnimationClip;
		std::shared_ptr<SampleTask> task = std::make_shared<SampleTask>();
		task->Time = Time;
		task->ClipID = node->Result.AnimationClip;
		task->NodeID = NodeID;
		context._graph->PushTask(task);
	}
}
