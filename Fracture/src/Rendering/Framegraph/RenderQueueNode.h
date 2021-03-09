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
		RenderQueueNode(std::string name,const std::shared_ptr<RenderBucket>& bucket);
		virtual void execute(Renderer& renderer);

		void render(Renderer& renderer);

		void renderOpaque(Renderer& renderer);
		void renderTranslusent(Renderer& renderer);
		
		std::shared_ptr<RenderBucket> GetBucket();


	private:
		int NumberBatches = 0;
		std::shared_ptr<RenderBucket> m_bucket;
		void WriteUniformData(Shader shader, std::string name, UniformValue value);
		void WriteUniformSampler(Shader shader, std::string name, std::shared_ptr<UniformValueSampler> value);

	};


}


#endif