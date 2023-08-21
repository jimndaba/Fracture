#include "FracturePCH.h"
#include "SelectorNodes.h"
#include "Animation/AnimationGraph.h"

Fracture::ISelectorNode::ISelectorNode() :
	IAnimationNode()
{
	
}

Fracture::FloatSelectorNode::FloatSelectorNode():ISelectorNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
			.PinType = PinValueType::Float,
			}
	};


	InputPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Option 1",
			.PinType = PinValueType::Float,
			},
		Pin{
		.PinID = Fracture::UUID(),
		.Name = "Option 2",
		.PinType = PinValueType::Float,
		},
		Pin{
		.PinID = Fracture::UUID(),
		.Name = "bool",
		.PinType = PinValueType::Bool,
		}
	};
	Name = "Float Selector";
}

void Fracture::FloatSelectorNode::Process(AnimationContext& context)
{
	const auto& selector = context._graph->GetNode(InputPins[2].NodeID);
	bool selection = selector->Result.BOOL;
	Result.FLOAT = context._graph->GetNode(InputPins[selection].NodeID)->Result.FLOAT;
}

Fracture::PoseSelectorNode::PoseSelectorNode():
	ISelectorNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
			.PinType = PinValueType::Float,
			}
	};


	InputPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Option 1",
			.PinType = PinValueType::Float,
			},
		Pin{
		.PinID = Fracture::UUID(),
		.Name = "Option 2",
		.PinType = PinValueType::Float,
		},
		Pin{
		.PinID = Fracture::UUID(),
		.Name = "bool",
		.PinType = PinValueType::Bool,
		}
	};
	Name = "Pose Selector";
}

void Fracture::PoseSelectorNode::Process(AnimationContext& context)
{
	const auto& selector = context._graph->GetNode(InputPins[2].NodeID);
	bool selection = selector->Result.BOOL;
	PoseNodeID = context._graph->GetNode(InputPins[selection].NodeID)->NodeID;
}
