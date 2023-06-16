#include "pch.h"
#include "ChromaticAberrationNode.h"
#include "rendering/Device.h"
#include "rendering/Pipeline.h"

Fracture::ChromaticAberrationNode::ChromaticAberrationNode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs) :RenderNode(context, name, inputs, outputs)
{
}


void Fracture::ChromaticAberrationNode::Setup()
{
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\Chromatic.frag";
		shader = Device::CreateShader(desc);
	}
}

void Fracture::ChromaticAberrationNode::Run(Scene& scene, CameraComponent* camera)
{
	OPTICK_CATEGORY("PresentPass", Optick::Category::Rendering);
	if (!Properties.ByPass)
	{
		const auto& prog = shader.get();
		const auto& read = Context.PostProcessReadTarget()->ColorAttachments[0]->RenderID;
		const auto& write = Context.PostProcessWriteTarget()->ColorAttachments[0]->RenderID;

		Context.BindRenderTarget(write);
		Context.BindShader(shader->Program);
		Context.SetUniforms([=]() { prog->SetTexture("srcTexture", read, 0); });
		Context.SetUniforms([=]() { prog->SetUniform("ByPass", Properties.ByPass); });
		Context.SetUniforms([=]() { prog->SetUniform("redOffset", Properties.redOffset); });
		Context.SetUniforms([=]() { prog->SetUniform("greenOffset", Properties.greenOffset); });
		Context.SetUniforms([=]() { prog->SetUniform("blueOffset", Properties.blueOffset); });

		Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
		Context.DrawArrays(DrawMode::Triangle, 0, 3);
		Context.BindRenderTarget(0);
	}

}
