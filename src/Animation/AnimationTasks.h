#pragma once
#ifndef ANIMATIONTASKS_H
#define ANIMATIONTASKS_H

#include "Animation/GraphNodes/IAnimationNode.h"

namespace Fracture
{


	struct IAnimationTask
	{
		virtual void Execute(AnimationContext& context) = 0;
	};

	struct SampleTask : IAnimationTask
	{
		SampleTask();
		void Execute(AnimationContext& context);
		Fracture::UUID ClipID;
		Fracture::UUID NodeID;
		float Time;
		float dt;
		float fps;
	};




	struct BlendTask : IAnimationTask
	{
		
		BlendTask();
		void Execute(AnimationContext& context);

		Fracture::UUID Pose1;
		Fracture::UUID Pose2;
		float factor;

		BlendFunctionType BlendFunction;
		BlendSpaceType BlendSpace;
	};


}

#endif