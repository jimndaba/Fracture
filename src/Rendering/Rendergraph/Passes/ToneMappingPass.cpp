#include "FracturePCH.h"
#include "ToneMappingPass.h"
#include "Assets/AssetManager.h"
#include "Rendering/RenderContext.h"
#include "Rendering/RenderCommands.h"
#include "Rendering/Shader.h"

void Fracture::ToneMappingPass::Setup()
{
	shader = AssetManager::Instance()->GetShader("ToneMapping");
}

void Fracture::ToneMappingPass::Render(PostProcessInfo info)
{
	RenderCommands::SetRenderTarget(info.cntxt, info.write_to);
	RenderCommands::UseProgram(info.cntxt, shader->Handle);
	RenderCommands::SetTexture(info.cntxt, shader.get(), "InColor", info.input_texture, 0);

	DrawArray cmd =
	{
		.mode = DrawMode::Triangles,
		.first = 0,
		.count = 3
	};

	RenderCommands::BindVertexArrayObject(info.cntxt, info.VAO);
	RenderCommands::DrawArray(info.cntxt, cmd);
}
