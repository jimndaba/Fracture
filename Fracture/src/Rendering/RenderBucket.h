#pragma once
#ifndef RENDERBUCKET_H
#define RENDERBUCKET_H

#include <vector>
#include <algorithm>
#include <string>
#include <memory>

namespace Fracture
{
	class RenderCommand;
	struct RenderInstancedElementsCommand;
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
		void pushCommand(std::shared_ptr<Mesh> mesh, std::shared_ptr<Fracture::Material> material,std::shared_ptr<TransformComponent> transform);
		void pushInstancedElementCommand(std::shared_ptr<RenderInstancedElementsCommand> command);
		void sort();
		void clear();

		std::vector<RenderCommand> getCommands(bool cull = false);
		std::vector<std::shared_ptr<RenderInstancedElementsCommand>> GetInstanced(bool cull = false);
		std::vector<std::string> Materials;
	

	private:
		std::vector<RenderCommand> m_commands;
		std::vector<std::shared_ptr<RenderInstancedElementsCommand>> m_InstancedElemtcommands;
	};

}

#endif