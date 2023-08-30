#include "FracturePCH.h"
#include "AnimationStateMachineNode.h"

Fracture::AnimationStateMachineNode::AnimationStateMachineNode()
{
	Name = "State Machine Node";
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Out Pose",
			.PinType = PinValueType::Pose
			}
	};
}

void Fracture::AnimationStateMachineNode::Process(AnimationContext& context)
{
	Result.PoseNodeID = Current_Node;
}
