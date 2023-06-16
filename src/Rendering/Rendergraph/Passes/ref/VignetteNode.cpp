#include "pch.h"
#include "VignetteNode.h"
#include "rendering/Device.h"
#include "rendering/Pipeline.h"

Fracture::VignetteNode::VignetteNode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs) :RenderNode(context, name, inputs, outputs)
{
}

void Fracture::VignetteNode::Setup()
{
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\Vignette.frag";
		shader = Device::CreateShader(desc);
	}
}

void Fracture::VignetteNode::Run(Scene& scene, CameraComponent* camera)
{
	OPTICK_CATEGORY("VignettePass", Optick::Category::Rendering);
	const auto& prog = shader.get();
	const auto& read = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;
	const auto& write = Context.PostProcessWriteTarget()->ColorAttachments[0]->RenderID;

	Context.BindRenderTarget(write);
	Context.BindShader(shader->Program);
	Context.SetUniforms([=]() { prog->SetTexture("srcTexture", read, 0); });
	Context.SetUniforms([=]() { prog->SetUniform("Inner", Properties.Inner); });
	Context.SetUniforms([=]() { prog->SetUniform("Outer", Properties.Outer); });
	Context.SetUniforms([=]() { prog->SetUniform("Roundness", Properties.Roundness); });
	Context.SetUniforms([=]() { prog->SetUniform("Smoothness", Properties.Smoothness); });

	Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
	Context.DrawArrays(DrawMode::Triangle, 0, 3);
	Context.BindRenderTarget(0);

}
