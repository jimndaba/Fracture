#pragma once
#ifndef RENDERCONTEXT_H
#define RENDERCONTEXT_H

#include "Command.h"

#include <stack>
#include "Viewport.h";
#include "Scissor.h"

namespace Fracture
{
	enum class DepthSortOrder
	{
		Front_To_Back,
		Back_To_Front
	};


	struct RenderContext
	{
		void BeginState(const SortKey& key);
		void EndState();

		void Begin();

		void Push(Fracture::Command& cmd);

		void Sort(DepthSortOrder order);

		void Render();

		std::vector<Fracture::Command> Commands;
		std::stack<SortKey> KeyStack;

		std::map<Fracture::UUID, std::map<Fracture::UUID, std::vector<glm::mat4>>> Renderable_batch;
		std::map<Fracture::UUID, std::vector<glm::vec4>> EntityIndeces;

		uint32_t CurrentProgram;

		Viewport ContextViewport;
		Scissor ContextScissor;
		bool CullFaceEnabled = true;
	};
}

#endif