#include "TestGraph.h"
#include "Rendering/Renderer.h"
#include "Passes/BufferClearPass.h"
#include "Passes/LambertianPass.h"
#include "Passes/EnvironmentPass.h"
#include "Rendering/Framegraph/PassLibrary/PickingPass.h"


std::unique_ptr<Fracture::PickingPass> Fracture::TestGraph::m_pickingPass;

Fracture::TestGraph::TestGraph(Renderer& renderer, std::string name):RenderGraph(renderer)
{
	{
		auto pass = std::make_unique<BufferClearPass>("clearRT");
		pass->SetSinkLinkage("buffer", "$.backbuffer");
		AppendPass(std::move(pass));
	}		

	{
		auto pass = std::unique_ptr<LambertianPass>(new LambertianPass("lambertian",renderer.m_opaqueBucket.get(),renderer.m_transparentBucket.get()));
		pass->SetSinkLinkage("renderTarget", "clearRT.buffer");
		AppendPass(std::move(pass));
	}		

	{
		auto pass = std::unique_ptr<EnvironmentPass>(new EnvironmentPass("environment"));
		pass->SetSinkLinkage("renderTarget", "lambertian.renderTarget");
		AppendPass(std::move(pass));
	}


	

	SetSinkTarget("backbuffer", "environment.renderTarget");
	Finalize();
}

int Fracture::TestGraph::PixelID(int x,int y)
{
	return 0;
	//return m_pickingPass->GetPixelInfo(x, y);
}

void  Fracture::TestGraph::Resize(int width,int height)
{
	//m_pickingPass->Resize(width, height);
}
