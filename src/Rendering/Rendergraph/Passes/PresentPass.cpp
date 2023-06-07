#include "FracturePCH.h"
#include "PresentPass.h"
#include "Rendering/VertexArray.h"
#include "Assets/AssetManager.h"

Fracture::PresentPass::PresentPass(const std::string& name,RenderContext* context, const PresentPassDef& info):IPass(name,context),definition(info)
{
}

void Fracture::PresentPass::Setup()
{
	VertexArrayCreationInfo info;
	GraphicsDevice::Instance()->CreateVertexArray(Vao, info);
}

void Fracture::PresentPass::Execute()
{
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_ColorBuffer");

	if (!global_color)
		return;

	Fracture::RenderCommands::ReleaseRenderTarget(Context);

	RenderCommands::SetCullMode(Context, CullMode::None);
	RenderCommands::SetViewport(Context, 1920, 1080, 0, 0);
	RenderCommands::SetScissor(Context, 1920, 1080, 0, 0);
	RenderCommands::ClearColor(Context, Colour::CornflourBlue);
	RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);


	Fracture::RenderCommands::UseProgram(Context, AssetManager::GetShader("Fullscreen")->Handle);
	Fracture::RenderCommands::SetTexture(Context, AssetManager::GetShader("Fullscreen").get(), "aDiffuse", global_color->ColorAttachments[0]->Handle, 0);

	DrawArray cmd =
	{
		.mode = DrawMode::Triangles,
		.first = 0,
		.count = 3
	};

	RenderCommands::BindVertexArrayObject(Context, Vao);
	RenderCommands::DrawArray(Context, cmd);


	Fracture::RenderCommands::ResetTextureUnits(Context, AssetManager::GetShader("Fullscreen").get());
	Fracture::RenderCommands::UseProgram(Context, 0);
}
