#pragma once
#ifndef ANIMBLENDER_H
#define ANIMBLENDER_H

#include "Animation/GraphNodes/IAnimationNode.h"

namespace Fracture
{
	struct AnimationBlender
	{
		AnimationBlender();

		void BlendSamples(BlendFunctionType func, BlendSpaceType space, PoseSample in1, PoseSample in2, float factor);

		PoseSample AdditiveBlend1D(PoseSample in1, PoseSample in2, float factor);

	};
}
#endif