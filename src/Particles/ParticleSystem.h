#pragma once
#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include "Assets/AssetRegistries.h"

namespace Fracture
{
	class RenderContext;
	struct Buffer;

	struct ParticleInstanceData
	{
		glm::vec4 Colour = glm::vec4(1.0, 1.0, 1.0, 1.0);
		glm::vec4 PositionScale = glm::vec4(0.0, 0.0, 0.0, 1.0);
	};

	struct ParticleRenderBatch
	{
		Fracture::UUID MaterialID;
		Fracture::UUID MeshID;
		std::vector<ParticleInstanceData> InstanceData;
		//std::vector<glm::vec4> Colors;
		std::vector<Particle> mPartilces;

		uint32_t VAO;
		std::shared_ptr<Buffer> Instance_Buffer;
		//std::shared_ptr<Buffer> Color_Buffer;
	};

	struct ParticleFXObservers
	{
		std::vector<Fracture::UUID> observers;
	};

	struct ParticleVertexData
	{
		glm::vec3 Position;
		glm::vec3 Normal = glm::vec3(1.0,0,0);
		glm::vec2 UV;
	};
	

	struct ParticleBillboard
	{
		ParticleBillboard()
		{
			float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
				// positions   // texCoords
				-0.5f, -0.5f,  0.0f, 0.0f,
				0.5f,  -0.5f, 0.0f, 1.0f,
				 -0.5f,  0.5f,  1.0f,0.0f,
				 0.5f,  0.5f,  1.0f, 1.0f

			};

			{
				ParticleVertexData v;
				v.Position = glm::vec3(-0.5f, -0.5f,0.0f);
			
				v.UV = glm::vec2(0.0f, 0.0f);
				mVertices.push_back(v);
			}
			{
				ParticleVertexData v;
				v.Position = glm::vec3(0.5f, -0.5f, 0.0f);				
				v.UV = glm::vec2(0.0f, 1.0f);
				mVertices.push_back(v);
			}
			{
				ParticleVertexData v;
				v.Position = glm::vec3(-0.5f, 0.5f, 0.0f);		
				v.UV = glm::vec2(1.0f, 0.0f);
				mVertices.push_back(v);
			}
			{
				ParticleVertexData v;
				v.Position = glm::vec3(0.5f, 0.5f, 0.0f);				
				v.UV = glm::vec2(1.0f, 1.0f);
				mVertices.push_back(v);
			}

		}
		uint32_t QuadVAO = 0;
		std::vector<ParticleVertexData> mVertices;
		std::shared_ptr<Buffer> Vertex_Buffer;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();

		void Init();
		void OnLoad();
		void OnSave();
		void Update(float dt);
		void BeginRender(Fracture::RenderContext* context);
		void Render(Fracture::RenderContext* context);
		void Shutdown();
		static void EmittParticles(Fracture::UUID entity);
		static void StopParticles(Fracture::UUID entity);
		static void UpdateObservers(Fracture::UUID fx);

		bool EmittParticleEmitter(ParticleEmitter& emitter, glm::vec3 Position, glm::quat Rotation);
		bool EmittParticleContius(ParticleEmitter& emitter, glm::vec3 Position, glm::quat Rotation);

		static ParticleFX* GetParticleFX(Fracture::UUID id);
		static ParticleFX* GetMainParticleFX(Fracture::UUID id);

		static std::map<Fracture::UUID, std::shared_ptr<ParticleFX>> mParticleFXs;
		static std::map<Fracture::UUID, std::shared_ptr<ParticleFX>> mParticleFXsLibrary;
		static std::map<UUID, ParticleFxRegistry> mParticleFxRegister;
		static std::map<UUID, ParticleFXObservers> mFxObserversList;
		static std::map<UUID, ParticleRenderBatch> mRenderBatch;
		int FindLastUsedParticleIndex(ParticleEmitter& emitter);

		std::unique_ptr<ParticleBillboard> mBillboard;


		void AddToBatch(const ParticleEmitter& fx);
	};



}

#endif