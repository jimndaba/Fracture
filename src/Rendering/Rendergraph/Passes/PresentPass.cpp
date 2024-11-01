#include "FracturePCH.h"
#include "PresentPass.h"


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
	const auto& aColor = GraphicsDevice::Instance()->PostProcessStack()->GetOutputImage();

	Fracture::RenderCommands::ReleaseRenderTarget(Context);

	RenderCommands::SetCullMode(Context, CullMode::None);
	RenderCommands::SetViewport(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetScissor(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::ClearColor(Context, Colour::CornflourBlue);
	RenderCommands::ClearTarget(Context, (uint32_t)Fracture::ClearFlags::Color | (uint32_t)Fracture::ClearFlags::Depth);


	Fracture::RenderCommands::UseProgram(Context, AssetManager::GetShader("Fullscreen")->Handle);
	Fracture::RenderCommands::SetTexture(Context, AssetManager::GetShader("Fullscreen").get(), "aDiffuse", aColor, 0);

	DrawArray cmd =
	{
		.mode = DrawMode::Triangles,
		.first = 0,
		.count = 3
	};

	RenderCommands::BindVertexArrayObject(Context, Vao);
	RenderCommands::DrawArray(Context, cmd);


	//Fracture::RenderCommands::ResetTextureUnits(Context, AssetManager::GetShader("Fullscreen").get());
	Fracture::RenderCommands::UseProgram(Context, 0);
}
