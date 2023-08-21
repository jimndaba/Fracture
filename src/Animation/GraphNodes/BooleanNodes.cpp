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

	

}

void Fracture::AndNode::Process(AnimationContext& context)
{
	Result.BOOL = true;
	for (const auto& node_id : mConditionsNodes)
	{
		auto node = std::static_pointer_cast<IBoolNode>(context._graph->GetNode(node_id));
		if (!node->Result.BOOL)
		{
			Result.BOOL = false;
		}		
	}
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
}

void Fracture::OrNode::Process(AnimationContext& context)
{
	Result.BOOL = false;
	for (const auto& node_id : mConditionsNodes)
	{
		auto node = std::static_pointer_cast<IBoolNode>(context._graph->GetNode(node_id));
		if (node->Result.BOOL)
		{
			Result.BOOL = true;
		}
	}
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
		.Name = "In",
		.PinType = PinValueType::Bool,
		}
	};
}

void Fracture::NotNode::Process(AnimationContext& context)
{
	auto node = std::static_pointer_cast<IBoolNode>(context._graph->GetNode(mConditionNode));
	Result.BOOL = !node->Result.BOOL;
}

Fracture::IBoolNode::IBoolNode():IAnimationNode()
{
	SetValueType(NodeValueType::BOOL);
}
