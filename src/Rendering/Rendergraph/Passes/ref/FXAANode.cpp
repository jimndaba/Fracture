#include "pch.h"
#include "FXAANode.h"
#include "rendering/Device.h"
#include "rendering/Pipeline.h"

Fracture::FXAANode::FXAANode(DeviceContext& context, const  std::string& name, const std::vector<std::string>& inputs, const std::vector<std::string>& outputs) :RenderNode(context, name, inputs, outputs)
{
}

void Fracture::FXAANode::Setup()
{
	{
		ShaderDescription desc;
		desc.VertexPath = "Content\\shaders\\postprocessing\\Fullscreen.vert";
		desc.FragmentPath = "Content\\shaders\\postprocessing\\FXAA.frag";
		shader = Device::CreateShader(desc);
	}
	{
		ResourceDescription res_desc;
		res_desc.Name = "FXAA";
		res_desc.format = Format::RGBA;
		res_desc.formartType = FormatType::Float;
		res_desc.TextureAttachmentTarget = AttachmentTarget::Color;
		res_desc.ColorBufferInternalformats = { InternalFormat::RGBA16 };
		res_desc.Width = Device::ScreenSize().x;
		res_desc.Height = Device::ScreenSize().y;
		res_desc.IsResizable = false;
		res_desc.TextureTarget = TextureTarget::Texture2D;
		res_desc.magFilter = TextureMagFilter::Linear;
		res_desc.minFilter = TextureMinFilter::Linear;
		res_desc.Type = ResourceType::RenderTarget;
		res_desc.Wrap = TextureWrap::ClampToEdge;
		Context.AddRenderResource(res_desc);
	}

}

void Fracture::FXAANode::Run(Scene& scene, CameraComponent* camera)
{
	OPTICK_CATEGORY("FXAAPass", Optick::Category::Rendering);
	const auto& prog = shader.get();
	const auto& read = Context.GetRenderReadTarget("BackBufferColor")->ColorAttachments[0]->RenderID;
	const auto& write = Context.GetRenderWriteTarget("FXAA")->RenderID; // Context.PostProcessWriteTarget()->RenderID;

	Context.BindRenderTarget(write);	
	Context.BindShader(shader->Program);
	Context.SetUniforms([=]()
		{
			prog->SetTexture("InColor", read, 0);
			prog->SetUniform("FXAA_SPAN_MAX", Properties.MaxSpans);

		});
	Context.BindVertexArray(Device::FullScreenPipeline()->Handle);
	Context.DrawArrays(DrawMode::Triangle, 0, 3);
}
