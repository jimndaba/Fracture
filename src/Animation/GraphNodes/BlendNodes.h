#pragma once
#ifndef BLENDNODES_H
#define BLENDNODES_H

#include "IAnimationNode.h"

namespace Fracture
{
	struct Blend1DNode : IAnimationNode
	{
		Blend1DNode();
		void Process(AnimationContext& context);

		BlendFunctionType BlendFunction;
		BlendSpaceType BlendSpace;
		float BlendFactor = 0;
	};
}
#endif