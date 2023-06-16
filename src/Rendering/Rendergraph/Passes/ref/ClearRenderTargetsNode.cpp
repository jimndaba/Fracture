#include "pch.h"
#include "ClearRenderTargetsNode.h"
#include "rendering/RenderTarget.h"

Fracture::ClearRenderTargetsNode::ClearRenderTargetsNode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs) :RenderNode(context, name, inputs, outputs)
{
}

void Fracture::ClearRenderTargetsNode::Setup()
{



}

void Fracture::ClearRenderTargetsNode::Run(Scene& scene, CameraComponent* camera)
{
	for (const auto& target : InputResources)
	{
		const auto& fbo = Context.GetRenderReadTarget(target.c_str());
		Context.BindRenderTarget(fbo->RenderID);
		Context.ClearBuffers((uint32_t)GLClearBufferBit::Depth | (uint32_t)GLClearBufferBit::Color);
		Context.BindRenderTarget(fbo->RenderID);
	}

	for (const auto& target : OutputResources)
	{
		const auto& fbo = Context.GetRenderWriteTarget(target.c_str());
		Context.BindRenderTarget(fbo->RenderID);
		Context.ClearBuffers((uint32_t)GLClearBufferBit::Depth | (uint32_t)GLClearBufferBit::Color);
		Context.BindRenderTarget(fbo->RenderID);
	}
}
