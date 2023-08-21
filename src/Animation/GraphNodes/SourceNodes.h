#pragma once
#ifndef PARAMETERNODE_H
#define PARAMETERNODE_H

#include "IAnimationNode.h"

#include "../AnimationState.h"

namespace Fracture
{
	struct ParameterSourceNode : IAnimationNode
	{
		ParameterSourceNode();
		void Process(AnimationContext& context);
		Fracture::UUID ParameterID;
		std::string Name;

		virtual void LinkResources();
	};

	struct PoseNode : IAnimationNode
	{
		PoseNode();
		void Process(AnimationContext& context);
		Fracture::UUID ParameterID;
		std::string Name;

		virtual void LinkResources();
	};
}

#endif