#pragma once
#ifndef BUFFERCLEARPASS_H
#define BUFFERCLEARPASS_H

#include "RenderPass.h"

namespace Fracture
{
	class BufferResource;
	class BufferClearPass: RenderPass
	{
	public:
		BufferClearPass(std::string Name);
		void Execute(Renderer& renderer) override;

	private:
		std::shared_ptr<BufferResource> resource;
	};

}

#endif