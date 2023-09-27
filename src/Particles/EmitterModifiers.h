#pragma once
#ifndef EMITTERMODIFIER_H
#define EMITTERMODIFIER_H


namespace Fracture
{
	struct ParticleEmitter;

	struct ModifierParams
	{
		ParticleEmitter& emitter;
		float dt;
		int particle_index;
	};

	struct EmitterModifier
	{
		virtual void Modify(ModifierParams params) = 0;
	};

	struct GravityModifier : EmitterModifier
	{
		void Modify(ModifierParams params);
		float GravityMultiplier = 1.0f;
	};

	struct ColourGradientModifier : EmitterModifier
	{
		void Modify(ModifierParams params);
		glm::vec4 StartColour = glm::vec4(1.0f);
		glm::vec4 EndColour = glm::vec4(1.0f);
	};

	struct ScaleModifier : EmitterModifier
	{
		void Modify(ModifierParams params);
		glm::vec3 StartScale = glm::vec3(1.0);
		glm::vec3 EndScale = glm::vec3(1.0);
	};





}

#endif