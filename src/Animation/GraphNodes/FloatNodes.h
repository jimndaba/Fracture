#pragma once
#ifndef FLOATNODES_H
#define FLOATNODES_H


#include "IAnimationNode.h"

namespace Fracture
{
	struct FloatEqualsNode : IOperationNode
	{
		FloatEqualsNode();
		void Process(AnimationContext& context);
		float RHfloat = 0.0f;
	};


	struct FloatGreaterThanNode : IOperationNode
	{
		FloatGreaterThanNode();
		void Process(AnimationContext& context);
		float RHfloat = 0.0f;
	};


	struct FloatLessThanNode : IOperationNode
	{
		FloatLessThanNode();
		void Process(AnimationContext& context);
		float RHfloat = 0.0f;
	};


	struct FloatClampNode : IOperationNode
	{
		FloatClampNode();
		void Process(AnimationContext& context);
		float Min = 0.0f;
		float Max = 1.0f;
	};

}
#endif