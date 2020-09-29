#pragma once
#ifndef RENDERBUCKET_H
#define RENDERBUCKET_H

#include <vector>
#include <algorithm>
#include <memory>

namespace Fracture
{
	class RenderCommand;
	class RenderInstancedCommand;
	class RenderComponent;
	class TransformComponent;
	class Mesh;
	class Material;

	class RenderBucket
	{
	public:
		RenderBucket();
		~RenderBucket();
		void pushCommand(RenderCommand command);
		void pushInstancedCommand(RenderInstancedCommand command);
		void pushCommand(std::shared_ptr<Mesh> mesh, std::shared_ptr<Fracture::Material> material,std::shared_ptr<TransformComponent> transform);

		void sort();
		void clear();

		std::vector<RenderCommand> getCommands(bool cull = false);
		std::vector< RenderInstancedCommand> getInstancedCommands();
	private:
		std::vector<RenderCommand> m_commands;
		std::vector< RenderInstancedCommand> m_InstancedCommands;

	};

}

#endif