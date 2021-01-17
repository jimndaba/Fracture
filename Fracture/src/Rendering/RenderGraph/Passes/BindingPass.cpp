#include "BindingPass.h"

Fracture::BindingPass::BindingPass(std::string name):RenderPass(name)
{
}

void Fracture::BindingPass::Execute(Renderer& renderer)
{

}

void Fracture::BindingPass::BindBufferResources(Renderer& renderer)
{
	if (renderTarget)
	{
		renderTarget->BindAsBuffer(renderer);
	}
}

void Fracture::BindingPass::BindAll(Renderer& renderer)
{
	BindBufferResources(renderer);
}

void Fracture::BindingPass::Finalise()
{
	renderTarget->Unbind();
}

