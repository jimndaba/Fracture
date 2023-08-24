#pragma once
#ifndef ANIMATIONCLIPNODE_H
#define ANIMATIONCLIPNODE_H

#include "PoseNodes.h"

namespace Fracture
{


	struct AnimationClipNode : IPoseNode
	{
		AnimationClipNode();
		void Process(AnimationContext& context);
		bool AnimationSet = false;
	};





}


#endif