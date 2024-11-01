#pragma once
#ifndef ANIMATIONSTATENODE_H
#define ANIMATIONSTATENODE_H

#include "Animation/GraphNodes/IAnimationNode.h"
#include "ValueNodes.h"

namespace Fracture
{

	struct AnimationStateNode : IPoseNode
	{
		AnimationStateNode();
		void Process(AnimationContext& context);

		Fracture::UUID mEntyConditionNode;
	};


}

#endif