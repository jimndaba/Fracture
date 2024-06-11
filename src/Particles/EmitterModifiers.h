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
		bool Enabled = true;
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

	struct AngularVeloctiyModifier : EmitterModifier
	{
		void Modify(ModifierParams params);
		float MinVelocity = 0.0f;
		float MaxVelocity = 1.0f;
	};

	struct WindModifier : EmitterModifier
	{
		void Modify(ModifierParams params);
		float Influence = 0.0f;		
	};

	struct NoiseModifier : EmitterModifier
	{
		void Modify(ModifierParams params);
		float MultiplierX = 0.0f;
		float MultiplierY = 0.0f;
		float MultiplierZ = 0.0f;
		float NoiseScale = 1.0f;
		float Frequency = 1.0f;

	private:
		int hash(int x, int y, int z);
		float grad(int hash, float x, float y, float z);
		float perlinNoise(float x, float y, float z);

		float fade(float t) {
			return t * t * t * (t * (t * 6 - 15) + 10);  // 6t^5 - 15t^4 + 10t^3
		}

		float lerp(float t, float a, float b) {
			return a + t * (b - a);
		}
	};

	struct VortexModifier : EmitterModifier
	{
		void Modify(ModifierParams params);
		UUID center_target;
		bool target_set = false;
		float VortexStrength;
	};

}

#endif