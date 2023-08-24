#pragma once
#ifndef SELECTORNODE_H
#define SELECTORNODE_H

#include "IAnimationNode.h"
#include "FloatNodes.h"
#include "AnimationClipNode.h"

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

	struct PoseSelectorNode :IPoseNode
	{
		PoseSelectorNode();
		void Process(AnimationContext& context);
		bool Value = false;
	};

}

#endif
