#include "FracturePCH.h"
#include "OutlinePass.h"
#include "Assets/AssetManager.h"

Fracture::OutlinePass::OutlinePass(const std::string& name, RenderContext* context, const OutlinePassDef& info) :
	IPass(name,context)
{
}

void Fracture::OutlinePass::Setup()
{
	mZShader = AssetManager::Instance()->GetShader("ZPrePass");
	mOShader = AssetManager::Instance()->GetShader("ZPrePass");
}

void Fracture::OutlinePass::Execute()
{
	
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalOutline);

	if (!global_color)
		return;


	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetScissor(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetCullMode(Context, CullMode::None);

	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	//RenderCommands::Enable(Context, Fracture::GLCapability::StencilTest);

	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Less);
	//RenderCommands::SetColorMask(Context, 0, 0, 0, 1);

	if (Context->mBatches.empty())
	{
		RenderCommands::ReleaseRenderTarget(Context);
		RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
		RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
		RenderCommands::Disable(Context, Fracture::GLCapability::StencilTest);
		return;
	}

	for (auto& batches : Context->mBatches)
	{
		if (batches.second.empty())
			continue;

		Fracture::RenderCommands::UseProgram(Context, mZShader->Handle);	
		for (auto batch : batches.second)
		{
			Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);
			for (const auto& drawcall : batch.second->OutlineDrawCalls)
			{
				//DrawElementsInstancedBaseVertex cmd;
				//cmd.basevertex = drawcall->basevertex;
				//cmd.instancecount = batch.second->Transforms.size();
				//cmd.indices = drawcall->SizeOfindices;
				//cmd.count = drawcall->SubMeshIndex;
				//Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
			}
			Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
		}
	}

	RenderCommands::ReleaseRenderTarget(Context);
	RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::Disable(Context, Fracture::GLCapability::StencilTest);
	RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
	
}
