#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include "Assets/AssetRegistries.h"

namespace Fracture
{
	class RenderContext;
	struct ParticleRenderBatch
	{
		std::vector<Particle> mParticles;
		std::vector<glm::mat4> transforms;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();

		void Init();
		void OnLoad();
		void OnSave();
		void Update(float dt);
		void Render(Fracture::RenderContext* context);
		void Shutdown();
		static void EmittParticles(Fracture::UUID entity);
		static void StopParticles(Fracture::UUID entity);

		bool EmittParticleEmitter(ParticleEmitter& emitter, glm::vec3 Position, glm::quat Rotation);
		bool EmittParticleContius(ParticleEmitter& emitter, glm::vec3 Position, glm::quat Rotation);

		static ParticleFX* GetParticleFX(Fracture::UUID id);
		static ParticleFX* GetMainParticleFX(Fracture::UUID id);

		static std::map<Fracture::UUID, std::shared_ptr<ParticleFX>> mParticleFXs;

		static std::map<Fracture::UUID, std::shared_ptr<ParticleFX>> mParticleFXsLibrary;

		static std::map<UUID, ParticleFxRegistry> mParticleFxRegister;
		

		int FindLastUsedParticleIndex(ParticleEmitter& emitter);

		uint32_t QuadVAO = 0;
		uint32_t QuadVBO = 0;
	};



}

#endif