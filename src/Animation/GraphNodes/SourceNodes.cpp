#include "FracturePCH.h"
#include "SourceNodes.h"



Fracture::ParameterSourceNode::ParameterSourceNode():
	IAnimationNode()
{

	OutPins = 
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Value",			
			.PinType = PinValueType::Bool,	
			}
	};


}

void Fracture::ParameterSourceNode::Process(AnimationContext& context)
{
}

void Fracture::ParameterSourceNode::LinkResources()
{
	OutPins[0].Resource = ParameterID;
}



Fracture::PoseNode::PoseNode() :
	IAnimationNode()
{
}

void Fracture::PoseNode::Process(AnimationContext& context)
{
}

void Fracture::PoseNode::LinkResources()
{
}
