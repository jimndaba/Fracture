#pragma once
#ifndef ANIMATIONGRAPHSERIALISER_H
#define ANIMATIONGRAPHSERIALISER_H


#include "Serialisation/Serialiser.h"

namespace Fracture
{
	struct AnimationGraph;

	struct AnimationGraphSerialiser : public ISerialiser
	{
		AnimationGraphSerialiser(ISerialiser::IOMode mode, ISerialiser::SerialiseFormat format);
		void WriteGraph(AnimationGraph* clip);

		void WriteParameters(AnimationGraph* graph);
	

		std::shared_ptr<AnimationGraph> ReadGraph();
		void ReadParameters(AnimationGraph* graph);
	

	};






}

#endif