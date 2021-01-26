#pragma once
#ifndef RENDERQUEUENODE_H
#define RENDERQUEUENODE_H
#include "FrameNode.h"

namespace Fracture
{
	class RenderBucket;
	class Shader;
	struct UniformValue;
	struct UniformValueSampler;


	class RenderQueueNode : public FrameNode
	{
	public:
		RenderQueueNode(std::string name);
		virtual void execute(Renderer& renderer);
		void AcceptBucket(RenderBucket* bucket);
		void render(Renderer& renderer);
		std::vector<RenderBucket*> m_buckets;
	private:
		int NumberBatches = 0;
		
		void WriteUniformData(Shader shader, std::string name, UniformValue value);
		void WriteUniformSampler(Shader shader, std::string name, std::shared_ptr<UniformValueSampler> value);

	};


}


#endif