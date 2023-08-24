#pragma once
#ifndef VALUENODES_H
#define VALUENODES_H

#include "Animation/GraphNodes/IAnimationNode.h"


namespace Fracture
{
	struct IValueNode : IAnimationNode
	{
		IValueNode();
		virtual void Process(AnimationContext& context) {};

		Fracture::UUID ParameterID;
	};



	struct FloatValueNode : IValueNode
	{
		FloatValueNode();
		void Process(AnimationContext& context);
		float Value = 0.0f;	
	};

	struct BoolValueNode : IValueNode
	{
		BoolValueNode();
		void Process(AnimationContext& context);
		bool Value = false;
	};

	struct IntValueNode : IValueNode
	{
		IntValueNode();
		void Process(AnimationContext& context);
		int Value = 0;
	};



}


#endif