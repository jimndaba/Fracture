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

		std::string Name = "And Node";

		std::vector<Fracture::UUID> mConditionsNodes;		
	};

	struct OrNode : IBoolNode
	{
		OrNode();
		void Process(AnimationContext& context);

		std::string Name = "Or Node";

		std::vector<Fracture::UUID> mConditionsNodes;	
	};

	struct NotNode : IBoolNode
	{
		NotNode();
		void Process(AnimationContext& context);

		std::string Name = "Not Node";

		Fracture::UUID mConditionNode;		
	};




}

#endif