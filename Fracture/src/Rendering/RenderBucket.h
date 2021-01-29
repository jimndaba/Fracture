#pragma once
#ifndef RENDERBUCKET_H
#define RENDERBUCKET_H

#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <map>

#include "DrawCommand.h"

namespace Fracture
{

	class RenderComponent;
	class TransformComponent;
	class Mesh;
	class Material;
	class RenderBatch;
	
	enum BucketType
	{
		Opaque,
		Transparent,
	};

	class RenderBucket
	{
	public:
		RenderBucket();
		~RenderBucket();
		void pushCommand(DrawCommand command);
		void pushCommand(uint32_t EntityID, std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, glm::mat4 transform);
		void sort();
		void clear();

		//std::vector<DrawCommand> getCommands(bool cull = false);

		std::vector<DrawCommand> getForwardRenderCommands();
		std::vector<DrawCommand> getAlphaRenderCommands();
		std::vector<DrawCommand> getShadowRenderCommands();

		//std::map<std::string, std::shared_ptr<RenderBatch>> getRenderBatches();
		std::vector<std::string> Materials;
		
		BucketType mType;

	private:
		//std::vector<DrawCommand> m_commands;

		std::vector<DrawCommand> m_Frowardcommands;
		std::vector<DrawCommand> m_Alphacommands;
		std::vector<DrawCommand> m_Shadowcommands;
		//std::map<std::string, std::shared_ptr<RenderBatch>> m_batches;


	};

}

#endif