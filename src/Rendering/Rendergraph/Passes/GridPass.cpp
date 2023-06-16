#include "FracturePCH.h"
#include "GridPass.h"
#include "Assets/AssetManager.h"
#include "SSAOPass.h"

Fracture::GridPass::GridPass(const std::string& name, RenderContext* context, const GridPassDef& info):IPass(name,context), params(info)
{
}


void Fracture::GridPass::Setup()
{
	VertexArrayCreationInfo info;
	GraphicsDevice::Instance()->CreateVertexArray(Vao, info);
}

void Fracture::GridPass::Execute()
{
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);

	if (!global_color)
		return;

	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::DepthFunction(Context,Fracture::DepthFunc::Less);
	RenderCommands::Enable(Context, Fracture::GLCapability::SmoothLines);
	RenderCommands::Enable(Context, Fracture::GLCapability::Blending);
	RenderCommands::BlendFunction(Context,Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);

	RenderCommands::Disable(Context, Fracture::GLCapability::FaceCulling);
	


	Fracture::RenderCommands::UseProgram(Context, AssetManager::GetShader("Grid")->Handle);
	
	Fracture::RenderCommands::BindVertexArrayObject(Context, Vao);
	
	DrawArraysInstancedBaseInstance cmd =
	{
		.mode = DrawMode::Triangles,
		.firstIndex = 0,
		.count = 6,
		.instanceCount = 1,
		.baseInstance = 0,
	};
	RenderCommands::DrawArraysInstancedBaseInstance(Context, cmd);

	RenderCommands::Disable(Context, GLCapability::DepthTest);
	RenderCommands::Disable(Context, GLCapability::SmoothLines);
	RenderCommands::Disable(Context, GLCapability::Blending);
	RenderCommands::ReleaseRenderTarget(Context);
}
