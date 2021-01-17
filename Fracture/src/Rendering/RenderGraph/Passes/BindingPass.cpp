#include "BindingPass.h"
#include "Rendering/Bindable.h"
#include "Logging/Logger.h"

Fracture::BindingPass::BindingPass(std::string name):RenderPass(name)
{
}

void Fracture::BindingPass::Execute(Renderer& renderer)
{

}

void Fracture::BindingPass::AddBind(std::shared_ptr<Bindable> bindable)
{
	m_bindables.push_back(std::move(bindable));
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
	for (auto& bind :m_bindables)
	{
		bind->bind();
	}
}

void Fracture::BindingPass::Finalise()
{
	RenderPass::Finalise();
	if (!renderTarget)
	{
		FRACTURE_ERROR("BindingPass [" + GetName() + "] needs at least one of a renderTarget");
	}

}

