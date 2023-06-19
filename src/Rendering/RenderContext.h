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
		std::shared_ptr<Buffer> VBO_Buffer;
		std::shared_ptr<Buffer> EBO_Buffer;

		void clear()
		{
			Transforms.clear();
			EntityIDs.clear();
		}
	};


	struct RenderContext
	{
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
	

		//std::map<Fracture::UUID, std::map<Fracture::UUID, std::vector<Fracture::UUID>>> Renderable_batch;
		//std::map<Fracture::UUID, std::vector<glm::vec4>> EntityIndeces;
		//std::map<Fracture::UUID, std::vector<glm::mat4>> MeshTransformBatch;

		uint32_t CurrentProgram;
		int ActiveTextureUnits = 0;
		Viewport ContextViewport;
		Scissor ContextScissor;
		bool CullFaceEnabled = true;
	};
}

#endif