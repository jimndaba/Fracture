#include "TestGraph.h"
#include "Passes/BufferClearPass.h"

Fracture::TestGraph::TestGraph(Renderer& renderer, std::string name):RenderGraph(renderer)
{
	{
		auto pass = std::make_unique<BufferClearPass>("clearRT");
		pass->SetSinkLinkage("buffer", "backbuffer.$");
		AppendPass(std::move(pass));
	}

	SetSinkTarget("backbuffer", "clearRT.buffer");
	Finalize();
}
