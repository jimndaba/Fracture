#pragma once
#ifndef SELECTORNODE_H
#define SELECTORNODE_H

#include "IAnimationNode.h"
#include "FloatNodes.h"

namespace Fracture
{	
	struct ISelectorNode : IAnimationNode
	{
		ISelectorNode();
		virtual void Process(AnimationContext& context) {};
	};

	struct FloatSelectorNode : ISelectorNode
	{
		FloatSelectorNode();
		void Process(AnimationContext& context);	
	};

	struct PoseSelectorNode : ISelectorNode
	{
		PoseSelectorNode();
		void Process(AnimationContext& context);
	};

}

#endif
