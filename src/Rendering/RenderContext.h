#pragma once
#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "Command.h"

#include <stack>
#include "Viewport.h";
#include "Scissor.h"

namespace Fracture
{
	struct StaticMesh;
	struct SubMesh;
	struct Buffer;
	struct MeshComponent;
	struct PrefabInstanceComponent;

	enum class DepthSortOrder
	{
		Front_To_Back,
		Back_To_Front
	};

	struct MeshDrawCall
	{
		UUID EntityID;
		UUID MaterialID;
		uint32_t MeshHandle;
		int basevertex = 0;	
		void* SizeOfindices = 0;
		int IndexCount = 0;
		int InstanceCount = 0;
		bool DrawToStencil;
		glm::vec4 IDColor;
		glm::mat4 model;
	};

	struct RenderBatch
	{
		std::vector<glm::mat4> Transforms;
		std::vector<glm::vec4> EntityIDs;
		std::vector<std::shared_ptr<MeshDrawCall>> OpaqueDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> ShadowDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> TransparentDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> OutlineDrawCalls;
		std::vector<SubMesh> Submeshes;
		
		uint32_t VAO;
		std::shared_ptr<Buffer> EntityID_Buffer;
		std::shared_ptr<Buffer> Matrix_Buffer;

		void clear()
		{
			Transforms.clear();
			EntityIDs.clear();
			OpaqueDrawCalls.clear();
			ShadowDrawCalls.clear();
			TransparentDrawCalls.clear();
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

		void Push(Fracture::Command& cmd);

		void Sort(DepthSortOrder order);

		void Render();

		void AddToBatch(MeshComponent* mesh,glm::mat4 transform,UUID Entity);
		void AddDrawCall(MeshComponent* mesh,glm::mat4 transform,UUID Entity);


		void AddToBatch(PrefabInstanceComponent* mesh,glm::mat4 transform,UUID Entity);
		void ResetBatches();
		void WriteToStencilBuffer(Fracture::UUID entity);

		bool IsWrittenOnStencil(Fracture::UUID entity);

		std::vector<Fracture::Command> Commands;
		std::stack<SortKey> KeyStack;
		std::map<Fracture::UUID,std::map<Fracture::UUID, std::shared_ptr<RenderBatch>>> mBatches;
		std::map<Fracture::UUID, bool> mStentilTestPass;
		
		std::vector<std::shared_ptr<MeshDrawCall>> OpaqueDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> ShadowDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> TransparentDrawCalls;
		std::vector<std::shared_ptr<MeshDrawCall>> OutlineDrawCalls;


	
		uint32_t CurrentProgram;
		int ActiveTextureUnits = 0;
		int currentIndex = 0;
		Viewport ContextViewport;
		Scissor ContextScissor;
		bool CullFaceEnabled = true;
		RenderContextFlags Flags;
		float deltaTime;
	
	};
}

#endif