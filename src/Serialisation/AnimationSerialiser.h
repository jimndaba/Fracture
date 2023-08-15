#pragma once
#ifndef ANIMATIONSERIALISER_H
#define ANIMATIONSERIALISER_H


#include "Serialisation/Serialiser.h"

namespace Fracture
{
	struct AnimationClip;
	
	struct AnimationSerialiser : public ISerialiser
	{
		AnimationSerialiser(ISerialiser::IOMode mode, ISerialiser::SerialiseFormat format);
		void WriteAnimation(AnimationClip* clip);
		std::shared_ptr<AnimationClip> ReadAnimation();
	};






}

#endif