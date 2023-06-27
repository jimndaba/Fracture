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
	struct Buffer;

	enum class DepthSortOrder
	{
		Front_To_Back,
		Back_To_Front
	};

	struct RenderBatch
	{
		std::vector<glm::mat4> Transforms;
		std::vector<glm::vec4> EntityIDs;;

		uint32_t VAO;
		std::shared_ptr<Buffer> EntityID_Buffer;
		std::shared_ptr<Buffer> Matrix_Buffer;

		void clear()
		{
			Transforms.clear();
			EntityIDs.clear();
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

		void Begin();

		void Push(Fracture::Command& cmd);

		void Sort(DepthSortOrder order);

		void Render();

		void AddToBatch(Fracture::UUID,StaticMesh* mesh, glm::mat4 transform,UUID Entity);
		void ResetBatches();

		std::vector<Fracture::Command> Commands;
		std::stack<SortKey> KeyStack;
		std::map <Fracture::UUID,std::map<Fracture::UUID, std::shared_ptr<RenderBatch>>> mBatches;
	
		uint32_t CurrentProgram;
		int ActiveTextureUnits = 0;
		Viewport ContextViewport;
		Scissor ContextScissor;
		bool CullFaceEnabled = true;
		RenderContextFlags Flags;

	
	};
}

#endif