#include "FracturePCH.h"
#include "ComposeRenderersPass.h"
#include "Assets/AssetManager.h"

Fracture::ComposeRenderersPass::ComposeRenderersPass(const std::string& name, RenderContext* context, const ComposeRenderersPassDef& info):IPass(name,context)
{
}

void Fracture::ComposeRenderersPass::Setup()
{
	VertexArrayCreationInfo info;
	GraphicsDevice::Instance()->CreateVertexArray(Vao, info);

	{
		RenderTargetCreationInfo info;

		Fracture::TextureCreationInfo desc;
		desc.Width = 1920;
		desc.Height = 1080;
		desc.AttachmentTrgt = Fracture::AttachmentTarget::Color;
		desc.format = Fracture::TextureFormat::RGBA;
		desc.internalFormat = Fracture::InternalFormat::RGBA8;
		desc.formatType = Fracture::TextureFormatType::UByte;
		desc.Name = "ColorBuffer";
		info.ColorAttachments.push_back(desc);

		GraphicsDevice::Instance()->CreateGlobalRenderTarget("FinalOut", info);
	}
}

void Fracture::ComposeRenderersPass::Execute()
{
	const auto& final_color = GraphicsDevice::Instance()->GetGlobalRenderTarget("FinalOut");
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_ColorBuffer");
	const auto& global_Debug = GraphicsDevice::Instance()->GetGlobalRenderTarget("Debug");

	if (!global_color)
		return;


	RenderCommands::SetRenderTarget(Context, final_color);
	RenderCommands::Enable(Context, Fracture::GLCapability::Blending);
	RenderCommands::BlendFunction(Context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);

	Fracture::RenderCommands::UseProgram(Context, AssetManager::GetShader("Compose")->Handle);
	
	Fracture::RenderCommands::SetTexture(Context, AssetManager::GetShader("Compose").get(), "aScene", global_color->ColorAttachments[0]->Handle, 0);
	
	if(global_Debug)
		Fracture::RenderCommands::SetTexture(Context, AssetManager::GetShader("Compose").get(), "aDebug", global_Debug->ColorAttachments[0]->Handle, 1);

	DrawArray cmd =
	{
		.mode = DrawMode::Triangles,
		.first = 0,
		.count = 3
	};

	RenderCommands::BindVertexArrayObject(Context, Vao);
	RenderCommands::DrawArray(Context, cmd);

	Fracture::RenderCommands::ResetTextureUnits(Context, AssetManager::GetShader("Compose").get());
	Fracture::RenderCommands::UseProgram(Context, 0);
	RenderCommands::Disable(Context, GLCapability::Blending);
	RenderCommands::ReleaseRenderTarget(Context);
}
