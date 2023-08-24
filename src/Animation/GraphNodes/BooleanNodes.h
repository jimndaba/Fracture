#pragma once
#ifndef BOOLEANNODES_H
#define BOOLEANNODES_H

#include "IAnimationNode.h"

namespace Fracture
{
	struct IBoolNode : IAnimationNode
	{
		IBoolNode();
		void Process(AnimationContext& context) {};
	};

	struct AndNode : IBoolNode
	{
		AndNode();
		void Process(AnimationContext& context);	
	};

	struct OrNode : IBoolNode
	{
		OrNode();
		void Process(AnimationContext& context);
	};

	struct NotNode : IBoolNode
	{
		NotNode();
		void Process(AnimationContext& context);	
	};




}

#endif