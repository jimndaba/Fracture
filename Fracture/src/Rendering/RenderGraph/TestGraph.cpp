#include "TestGraph.h"
#include "Rendering/Renderer.h"
#include "Passes/BufferClearPass.h"
#include "Passes/LambertianPass.h"

Fracture::TestGraph::TestGraph(Renderer& renderer, std::string name):RenderGraph(renderer)
{
	{
		auto pass = std::make_unique<BufferClearPass>("clearRT");
		pass->SetSinkLinkage("buffer", "backbuffer.$");
		AppendPass(std::move(pass));
	}

	{

		auto pass = std::unique_ptr<LambertianPass>(new LambertianPass("lambertian",renderer.m_opaqueBucket.get(),renderer.m_transparentBucket.get()));
		pass->SetSinkLinkage("renderTarget", "buffer.clearRT");
		AppendPass(std::move(pass));
	}

	SetSinkTarget("backbuffer", "renderTarget.lambertian");
	Finalize();
}
