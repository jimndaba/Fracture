#pragma once
#ifndef RENDERBUCKET_H
#define RENDERBUCKET_H

#include <vector>
#include <algorithm>
#include <memory>

namespace Fracture
{
	class RenderCommand;
	class RenderComponent;
	class TransformComponent;

	class RenderBucket
	{
	public:
		RenderBucket();
		~RenderBucket();
		void pushCommand(RenderCommand command);
		void pushCommand(std::shared_ptr<RenderComponent> render, std::shared_ptr<TransformComponent> transform);

		void sort();
		void clear();

		std::vector<RenderCommand> getCommands(bool cull = false);

	private:
		std::vector<RenderCommand> m_commands;

	};

}

#endif