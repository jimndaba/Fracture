#include "BufferClearPass.h"
#include "BufferResource.h"
#include "DirectBufferSink.h"


Fracture::BufferClearPass::BufferClearPass(std::string Name):RenderPass(std::move(Name))
{
	RegisterSink(Fracture::DirectBufferSink<BufferResource>::Make("buffer", resource));
}

void Fracture::BufferClearPass::Execute(Renderer& renderer)
{
	resource->Clear(renderer);
}
