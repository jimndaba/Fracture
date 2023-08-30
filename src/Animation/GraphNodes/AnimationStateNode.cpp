#include "FracturePCH.h"
#include "AnimationStateNode.h"
#include "Animation/AnimationGraph.h"
#include "Animation/AnimationTasks.h"
#include "AnimationClipNode.h"

Fracture::AnimationStateNode::AnimationStateNode()
{
	Name = "State Node";
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Out Pose",
			.PinType = PinValueType::Pose
			}
	};

	
}

void Fracture::AnimationStateNode::Process(AnimationContext& context)
{
	HasAnimation = true;
	CurrentAnimation = Result.AnimationClip;
	std::shared_ptr<SampleTask> task = std::make_shared<SampleTask>();
	task->Time = Time;
	task->ClipID = CurrentAnimation;
	task->NodeID = NodeID;
	context._graph->PushTask(task);
}
