#include "FracturePCH.h"
#include "BooleanNodes.h"
#include "Animation/AnimationGraph.h"

Fracture::AndNode::AndNode() : IBoolNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
			.PinType = PinValueType::Bool,
			}
	};

	
	InputPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "LhV",
			.PinType = PinValueType::Bool,
			},
		Pin{
		.PinID = Fracture::UUID(),
		.Name = "RhV",
		.PinType = PinValueType::Bool,
		}
	};

	Name = "And Node";

}

void Fracture::AndNode::Process(AnimationContext& context)
{
	const auto& lh = context._graph->GetNode(InputPins[0].NodeID);
	const auto& rh = context._graph->GetNode(InputPins[1].NodeID);	
	Result.BOOL = lh->Result.BOOL && rh->Result.BOOL;
}

Fracture::OrNode::OrNode() : IBoolNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
			.PinType = PinValueType::Bool,
			}
	};

	InputPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "LhV",
			.PinType = PinValueType::Bool,
			},
		Pin{
		.PinID = Fracture::UUID(),
		.Name = "RhV",
		.PinType = PinValueType::Bool,
		}
	};

	Name = "Or Node";
}

void Fracture::OrNode::Process(AnimationContext& context)
{
	const auto& lh = context._graph->GetNode(InputPins[0].NodeID);
	const auto& rh = context._graph->GetNode(InputPins[1].NodeID);
	Result.BOOL = lh->Result.BOOL || rh->Result.BOOL;
}

Fracture::NotNode::NotNode() : IBoolNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
			.PinType = PinValueType::Bool,
			}
	};

	InputPins =
	{
		Pin{
		.PinID = Fracture::UUID(),
		.Name = "Bool",
		.PinType = PinValueType::Bool,
		}
	};

	Name = "Not Node";
}

void Fracture::NotNode::Process(AnimationContext& context)
{
	const auto& lh = context._graph->GetNode(InputPins[0].NodeID);
	Result.BOOL = !lh->Result.BOOL;
}

Fracture::IBoolNode::IBoolNode():IAnimationNode()
{
	SetValueType(NodeValueType::BOOL);
}
