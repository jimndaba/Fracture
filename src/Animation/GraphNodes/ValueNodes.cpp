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
	if (context._graph->Parameters.find(ParameterID) != context._graph->Parameters.end())
	{
		Result.FLOAT = context._graph->Parameters[ParameterID]->Value.FLOAT;		
	}
	else
	{
		FRACTURE_ERROR("Float Value node found");
	}
}

Fracture::BoolValueNode::BoolValueNode() : IValueNode()
{
	InputPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "bool",
			.PinType = PinValueType::Bool,
			}
	};
}

void Fracture::BoolValueNode::Process(AnimationContext& context)
{
	if(context._graph->GetNode(InputPins[0].NodeID))
		Result.BOOL = context._graph->GetNode(InputPins[0].NodeID)->Result.BOOL;
}

Fracture::IntValueNode::IntValueNode() : IValueNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "int",
			.PinType = PinValueType::Int,
			}
	};
}

void Fracture::IntValueNode::Process(AnimationContext& context)
{
	if (context._graph->Parameters.find(ParameterID) != context._graph->Parameters.end())
	{
		Result.INT = context._graph->Parameters[ParameterID]->Value.INTERGER;
	}
	else
	{
		FRACTURE_ERROR("Int Value node found");
	}
}

Fracture::BoolParameterNode::BoolParameterNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "bool",
			.PinType = PinValueType::Bool,
			}
	};
}

void Fracture::BoolParameterNode::Process(AnimationContext& context)
{
	if (context._graph->Parameters.find(ParameterID) != context._graph->Parameters.end())
	{
		Result.BOOL = context._graph->Parameters[ParameterID]->Value.BOOLEAN;
	}
	else
	{
		FRACTURE_ERROR("Bool Value node found");
	}
}
