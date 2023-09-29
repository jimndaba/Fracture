#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H


namespace Fracture
{
	struct EmitterModifier;

	struct Particle
	{
		glm::vec3 Position = glm::vec3(0);
		glm::vec3 Velocity = glm::vec3(0);
		glm::vec3 Rotation = glm::vec3(0);
		glm::vec3 Scale = glm::vec3(1.0f);

		glm::vec4 Color;
		float LifeSpan = 1.0f;		
		float Weight = 1.0f;

		float TimeAlive = 0;
		bool IsAlive = false;
		float DistanceToCamera = 0.0f;

		bool operator < (const Particle that) {
			// Sort in reverse order : far particles drawn first.
			return DistanceToCamera > that.DistanceToCamera;
		}
	};

	struct ParticleEmitter
	{
		enum class EmitterTypeOption 
		{
			Billboard,
			Mesh,
			Circle
		};

		enum class BlendingModeOption
		{
			Additive,
			Linear,
			Multiply
		};

		enum class SubEmittStageOption
		{
			OnStart,
			During,
			OnDie
		};

		std::string Name = "Emitter";
		Fracture::UUID MaterialID;
		Fracture::UUID MeshID;

		bool HasMaterial = false;
		bool HasMesh = false;

		EmitterTypeOption EmitterType = EmitterTypeOption::Billboard;
		BlendingModeOption BlendMode = BlendingModeOption::Linear;
		SubEmittStageOption EmmitStage = SubEmittStageOption::OnDie;

		int ParticlesPerSec = 5;
		float SpawnRate = 1.0f;
		int AliveParticles = 0;
		int LastUsedParticle = 0;

		float TimeSinceLastSpawn = 0;
		float StartOffset = 0;
		float EndTime = 1.0f;
		float Duration = 1.0f;
		float CurrentTime = 0.0f;
		float ParticleLifeSpan = 1.0f;
		float ParticleSpeed = 1.0f;

		bool HasParticleTexture = false;
		bool IsTextureAtlas = false;
		Fracture::UUID ParticleTexture;

		glm::vec3 ParticleVelocity = glm::vec3(0);
		glm::vec3 PositionOffset = glm::vec3(0);	
		glm::vec3 ParticleSpread = glm::vec3(0);	
		float EmittionAngle = 90.0f;

		std::vector<Particle> Particles;
		int ParticlePoolIndex = 0;
		std::vector<std::shared_ptr<EmitterModifier>> mModifiers;
		std::vector<ParticleEmitter> mSubEmitters;
	};

	struct ParticleFX
	{		
		ParticleFX(){}
		ParticleFX(ParticleFX& other)		
		{
			ID = other.ID;
			Name = other.Name;
			PlayMode = other.PlayMode;
			FxState = other.FxState;
			Duration = other.Duration;
			CurrentTime = other.CurrentTime;
			mEmitters = other.mEmitters;
		}

		enum class PlayModeOptions
		{
			Once,
			Looping 
		};

		enum class StateOptions
		{
			Stopped,
			Playing
		};


		Fracture::UUID ID;
		std::string Name;
		bool IsDirty = false;

		PlayModeOptions PlayMode = PlayModeOptions::Looping;
		StateOptions FxState = StateOptions::Playing;

		float Duration = 1.0f;
		float CurrentTime = 0.0f;

		std::map<int, ParticleEmitter> mEmitters;		
		std::vector<int> mEmittersOrder;
		std::vector<int> mAvialableOrderIndex;
	};
}

#endif