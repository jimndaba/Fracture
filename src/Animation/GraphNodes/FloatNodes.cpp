#include "FracturePCH.h"
#include "FloatNodes.h"
#include "Animation/AnimationGraph.h"


Fracture::FloatEqualsNode::FloatEqualsNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
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
	Name = "Float Equals";
}

void Fracture::FloatEqualsNode::Process(AnimationContext& context)
{
	const auto& lh = context._graph->GetNode(InputPins[0].NodeID);
	if (InputPins[1].IsValueSet)
	{
	 	RHfloat = context._graph->GetNode(InputPins[1].NodeID)->Result.FLOAT;
	}
	
	Result.BOOL = lh->Result.FLOAT == RHfloat;
}

Fracture::FloatGreaterThanNode::FloatGreaterThanNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
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

	Name = "Float GreaterThan";
}

void Fracture::FloatGreaterThanNode::Process(AnimationContext& context)
{
	const auto& lh = context._graph->GetNode(InputPins[0].NodeID);
	if (InputPins[1].IsValueSet)
	{
		RHfloat = context._graph->GetNode(InputPins[1].NodeID)->Result.FLOAT;
	}

	Result.BOOL = lh->Result.FLOAT > RHfloat;
}

Fracture::FloatLessThanNode::FloatLessThanNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
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

	Name = "Float LessThan";
}

void Fracture::FloatLessThanNode::Process(AnimationContext& context)
{
	const auto& lh = context._graph->GetNode(InputPins[0].NodeID);
	if (InputPins[1].IsValueSet)
	{
		RHfloat = context._graph->GetNode(InputPins[1].NodeID)->Result.FLOAT;
	}
	Result.BOOL = lh->Result.FLOAT < RHfloat;
}

Fracture::FloatClampNode::FloatClampNode()
{
	OutPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "Result",
			}
	};
	InputPins =
	{
		Pin{
			.PinID = Fracture::UUID(),
			.Name = "float",
			.PinType = PinValueType::Bool,
			},	
	};
}

void Fracture::FloatClampNode::Process(AnimationContext& context)
{
	const auto& value = context._graph->GetNode(InputPins[0].NodeID)->Result.FLOAT;
	Result.FLOAT = glm::clamp(value, Min, Max);
}
