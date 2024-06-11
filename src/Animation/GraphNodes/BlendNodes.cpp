#include "FracturePCH.h"
#include "BlendNodes.h"
#include "Animation/AnimationTasks.h"
#include "Animation/AnimationGraph.h"
#include "Animation/GraphNodes/ValueNodes.h"
#include "Animation/AnimationState.h"
#include "PoseNodes.h"

Fracture::Blend1DNode::Blend1DNode():IAnimationNode()
{
	Name = "Blend 1D";
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Out Pose",
			.PinType = PinValueType::Pose,
			}
	};


	InputPins =
	{
		Pin
		{
			.PinID = Fracture::UUID(),
			.Name = "Pose 1",
			.PinType = PinValueType::Pose,
		},
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Pose 2",
			.PinType = PinValueType::Pose,
		},
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "BlendFactor",
			.PinType = PinValueType::Float,
		}
	};
}

void Fracture::Blend1DNode::Process(AnimationContext& context)
{	
	
	std::shared_ptr<BlendTask> task = std::make_shared<BlendTask>();
	task->Pose1 = InputPins[0].NodeID;
	task->Pose2 = InputPins[1].NodeID;

	if (InputPins[2].IsValueSet)
	{
		auto node = context._graph->GetNode(InputPins[2].NodeID);
		if(node)
			BlendFactor = node->Result.FLOAT;
	}
	
	task->factor = BlendFactor;

	context._graph->PushTask(task);
}
