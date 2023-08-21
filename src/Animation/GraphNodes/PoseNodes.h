#pragma once
#ifndef POSENODES_H
#define POSENODES_H

#include "IAnimationNode.h"

namespace Fracture
{
	struct ReferencePoseNode : IPoseNode
	{
		ReferencePoseNode();
		void Process(AnimationContext& context);
	};

	struct AnimationPoseNode : IPoseNode
	{
		AnimationPoseNode();
		void Process(AnimationContext& context);
	};



}
#endif