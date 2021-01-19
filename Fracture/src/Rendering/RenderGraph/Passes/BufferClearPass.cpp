#include "BufferClearPass.h"
#include "../DirectBufferSink.h"
#include "../BufferResource.h"
#include "Rendering/Renderer.h"

Fracture::BufferClearPass::BufferClearPass(std::string Name):RenderPass(std::move(Name))
{
	RegisterSink(DirectBufferSink<BufferResource>::Make("buffer", resource));	
	RegisterSource(DirectBufferSource<BufferResource>::Make("buffer", resource));
}

void Fracture::BufferClearPass::Execute(Renderer& renderer)
{
	resource->Clear(renderer);
}
