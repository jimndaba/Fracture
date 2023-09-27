#pragma once
#ifndef PARTICLEFXSERIALISER_H
#define PARTICLEFXSERIALISER_H

#include "Serialiser.h"

namespace Fracture
{
	struct ParticleFX;
	struct GravityModifier;
	struct ColourGradientModifier;
	struct ScaleModifier;
	struct ParticleEmitter;
	struct EmitterModifier;

	struct ParticleFxSerialiser : public ISerialiser
	{

		ParticleFxSerialiser(IOMode mode, SerialiseFormat format);

		void WriteParticleFX(ParticleFX* fx);
		std::shared_ptr<ParticleFX> ReadParticleFX();

		void SerialiseEmitter(ParticleEmitter& emitter);
		void SerialiseModifier(GravityModifier* modifier);
		void SerialiseModifier(ColourGradientModifier* modifier);
		void SerialiseModifier(ScaleModifier* modifier);


		void ReadGravityModifierIfExists(ParticleEmitter* em);
		void ReadColourGradientModifierIfExists(ParticleEmitter* em);
		void ReadScaleModifierIfExists(ParticleEmitter* em);
		void ReadEmitter(ParticleFX* fx);
		void ReadSubEmitter(ParticleEmitter* fx);

		template <class T>
		void WriteModifierOfType(const std::shared_ptr<EmitterModifier>& emitter);

	};


	template<class T>
	inline void ParticleFxSerialiser::WriteModifierOfType(const std::shared_ptr<EmitterModifier>& modifier)
	{
		if (dynamic_cast<T*>(modifier.get()))
		{
			SerialiseModifier(dynamic_cast<T*>(modifier.get()));
		}
	}

}

#endif