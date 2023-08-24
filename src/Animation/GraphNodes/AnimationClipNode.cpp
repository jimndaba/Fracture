#include "FracturePCH.h"
#include "AnimationClipNode.h"

Fracture::AnimationClipNode::AnimationClipNode() : IPoseNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Pose",
			.PinType = PinValueType::Pose
			}
	};
	Name = "AnimationClip";
}

void Fracture::AnimationClipNode::Process(AnimationContext& context)
{
}
