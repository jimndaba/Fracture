#include "FracturePCH.h"
#include "ValueNodes.h"
#include "Animation/AnimationGraph.h"
#include "Animation/AnimationState.h"

Fracture::IValueNode::IValueNode():IAnimationNode()
{
}

Fracture::FloatValueNode::FloatValueNode() 
	: IValueNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "float",
			.PinType = PinValueType::Float,
			}
	};
}

void Fracture::FloatValueNode::Process(AnimationContext& context)
{	
	if (context._graph->Parameters.find(this->ParameterID) != context._graph->Parameters.end())
	{
		this->Result.FLOAT = context._graph->Parameters[this->ParameterID]->Value.FLOAT;
		Value = context._graph->Parameters[this->ParameterID]->Value.FLOAT;
	}
	else
	{
		FRACTURE_ERROR("Float Value node found");
	}
}
