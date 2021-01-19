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
		void pushCommand(std::shared_ptr<Mesh> mesh, std::shared_ptr<Fracture::Material> material,std::shared_ptr<TransformComponent> transform);
		void sort();
		void clear();

		std::vector<DrawCommand> getCommands(bool cull = false);
		std::map<std::string, std::shared_ptr<RenderBatch>> getRenderBatches();
		std::vector<std::string> Materials;
		
		BucketType mType;

	private:
		std::vector<DrawCommand> m_commands;
		std::map<std::string, std::shared_ptr<RenderBatch>> m_batches;


	};

}

#endif