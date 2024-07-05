#include "FracturePCH.h"
#include "ChromaticAberration.h"

void Fracture::ChromaticAberrationPass::Setup()
{	
	shader = AssetManager::Instance()->GetShader("Chromatic");
}

bool Fracture::ChromaticAberrationPass::Render(PostProcessInfo info)
{
	OPTICK_EVENT();
	if (!GraphicsDevice::Instance()->RenderSettings.ChromaticAb_Enabled)
		return false;


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
	return true;
}
