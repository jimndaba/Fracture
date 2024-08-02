#pragma once
#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "Viewport.h"
#include "Scissor.h"
#include "Command.h"
#include "AABB.h"
#include "RenderCommands.h"

namespace Fracture
{
	struct StaticMesh;
	struct SubMesh;
	struct Buffer;
	struct MeshComponent;
	struct PrefabInstanceComponent;
	struct TerrainComponent;

	struct MeshSortKey
	{		
		uint16_t ShaderIndex = 0;
		uint16_t MaterialIndex = 0;
		uint16_t MeshIndex = 0;
		float Depth = 0.0f;

		bool operator < (const MeshSortKey& r) const;
		bool operator > (const MeshSortKey& r) const;
	};

	struct MeshDrawCall
	{
		Fracture::DrawMode DrawCallPrimitive = DrawMode::Triangles;
		Fracture::DrawCommandType CallType = Fracture::DrawCommandType::DrawElementsInstancedBaseVertex;
		UUID EntityID;
		UUID MaterialID;
		uint32_t GPUMaterialIndex;
		uint32_t MeshHandle;
		int baseIndex = 0;
		int basevertex = 0;	
		void* SizeOfindices = 0;
		int IndexCount = 0;
		int InstanceCount = 0;
		bool DrawToStencil;
		glm::vec4 IDColor;
		glm::mat4 model;
		AABB aabb;
		MeshSortKey Key;
	};

	struct TerrainDrawCall : MeshDrawCall
	{
		int NUM_STRIPS;
		int NUM_VERTS_PER_STRIP;
	};

	struct GPUMaterial
	{
		int TextureSpace;
		float Tiling;
		float pAO;
		float pMetalness;
		float pRoughness;
		float pEmissionStrength;
		float SpecularLevel;
		float SpecularIntensity;
		glm::vec4 pDiffuse;
		glm::vec4 pEmission;

		int AlbedoFlag;
		int SpecularFlag;
		int NormalFlag;
		int RoughnessFlag;
		int MetalnessFlag;
		int AOFlag;
		int EmissionFlag;
		float pad;
		//int HeightMapFlag;
		//int MixMapFlag;
		//glm::vec2 _pad;
	};

	struct RenderBatch
	{
		std::vector<glm::mat4> Transforms;
		std::vector<glm::vec4> EntityIDs;				
		std::vector<std::shared_ptr<MeshDrawCall>> OutlineDrawCalls;
		
		uint32_t VAO;
		uint32_t GPUMaterialIndex;
		Fracture::DrawMode DrawCallPrimitive = DrawMode::Triangles;
		Fracture::DrawCommandType CallType = Fracture::DrawCommandType::DrawElementsInstancedBaseVertex;
		int basevertex = 0;
		void* SizeOfindices = 0;
		int IndexCount = 0;
		int InstanceCount = 0;

		bool IsTranslucent = false;
		bool CastShadows = false;

		std::shared_ptr<Buffer> EntityID_Buffer;
		std::shared_ptr<Buffer> Matrix_Buffer;

		void clear()
		{
			Transforms.clear();
			EntityIDs.clear();
			OutlineDrawCalls.clear();
		}
	};

	enum RenderContextFlags : uint8_t
	{
		RenderContextFlag_IsResizable = (uint8_t)1 << 0, //Same as 1
		RenderContextFlag_Shadows = (uint8_t)1 << 1, //Same as 2
		RenderContextFlag_Opaque = (uint8_t)1 << 2, //Same as 4
		RenderContextFlag_PostProcessing = (uint8_t)1 << 3, //Same as 8
		RenderContextFlag_SortOrderFrontToBack = (uint8_t)1 << 4,//Same as 8
		RenderContextFlag_SortOrderBackToFront = (uint8_t)1 << 5, //Same as 8
		RenderContextFlag_DrawRenderIDs = (uint8_t)1 << 6,//Same as 8
		RenderContextFlag_None = (uint8_t)1 << 7
	
	};

	inline RenderContextFlags operator|(const RenderContextFlags& lhs, const RenderContextFlags& rhs)
	{
		return static_cast<RenderContextFlags>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
	}

	inline RenderContextFlags operator&(const RenderContextFlags& lhs, const RenderContextFlags& rhs)
	{
		return static_cast<RenderContextFlags>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
	}
	
	struct RenderContextInfo
	{
		bool IsResizable = false;

	};

	struct RenderContext
	{
		RenderContext(RenderContextFlags flags);
		void BeginState(const SortKey& key);
		void EndState();

		void BeginScene();
		void EndScene();

		void Push(Fracture::Command& cmd);

		void Sort(DepthSortOrder order);

		void Render();

		void AddToBatch(StaticMesh* mesh, Fracture::UUID material, glm::mat4 transform, UUID Entity = 0);
		void AddToBatch(SubMesh* mesh, Fracture::UUID meshID, Fracture::UUID material, std::vector<glm::mat4> Batchtransform);
		void AddToBatch(MeshComponent* mesh,glm::mat4 transform,UUID Entity);
		void AddDrawCall(MeshComponent* mesh,glm::mat4 transform,UUID Entity);
		void AddDrawCall(TerrainComponent* mesh,glm::mat4 transform,UUID Entity);

		void DrawOutlines(UUID Entity);
		void DrawPrefabOutlines(UUID Entity);

		void AddToBatch(PrefabInstanceComponent* mesh,glm::mat4 transform,UUID Entity);

		void CreateBatchIfMissing(UUID MaterialID, UUID MeshID);
		void SubmitMaterialstoGPU(UUID MaterialID);
		void ResetBatches();
		void WriteToStencilBuffer(Fracture::UUID entity);

		bool IsWrittenOnStencil(Fracture::UUID entity);

		std::vector<Fracture::Command> Commands;
		std::stack<SortKey> KeyStack;
		std::map<Fracture::UUID,std::map<Fracture::UUID, std::shared_ptr<RenderBatch>>> mBatches;
		std::map<Fracture::UUID, bool> mStentilTestPass;
		
		std::vector<DrawElementsIndirectCommand> IndirectTerrains;
		std::vector<std::shared_ptr<MeshDrawCall>> OpaqueDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> ShadowDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> TransparentDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> OutlineDrawCalls;
		
		std::vector<GPUMaterial> MaterialGPUData;
		std::map<UUID, int> MaterialIndexMap;
		std::unordered_map<GLCapability, bool> mContextState;
	
		uint32_t CurrentProgram;
		uint32_t CurrentRenderTarget;
		uint32_t CurrentRenderVAO;
		std::vector<int> ActiveTextureUnits;

		int currentIndex = 0;
		Viewport ContextViewport;
		Scissor ContextScissor;
		bool CullFaceEnabled = true;
		bool TerrainEdititing = false;
		RenderContextFlags Flags;
		float deltaTime;
		int MaxBufferSize = 10240;
	
	};
}

#endif