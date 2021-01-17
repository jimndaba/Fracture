#pragma once
#ifndef RENDERQUEUEPASS_H
#define RENDERQUEUEPASS_H

#include "BindingPass.h"


namespace Fracture
{
	class RenderBucket;
	class Shader;
	struct UniformValue;
	struct UniformValueSampler;

	class RenderQueuePass : public BindingPass
	{
	public:
		RenderQueuePass(std::string Name);
		void Execute(Renderer& renderer) override;
		void Reset() override;
		void AcceptBucket(RenderBucket* bucket);
	private:
		int NumberBatches = 0;
		std::vector<RenderBucket*> m_buckets;
		void WriteUniformData(Shader shader, std::string name, UniformValue value);
		void WriteUniformSampler(Shader shader, std::string name, std::shared_ptr<UniformValueSampler> value);
	};
}

#endif