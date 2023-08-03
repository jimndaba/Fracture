#include "FracturePCH.h"
#include "ZPrePass.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

Fracture::ZPrePass::ZPrePass(const std::string& name, RenderContext* context, const ZPrePassDef& info) :IPass(name, context),Properties(info)
{
}

void Fracture::ZPrePass::Setup()
{
	mShader = AssetManager::Instance()->GetShader("ZPrePass");
}

void Fracture::ZPrePass::Execute()
{
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);

	if (!global_color)
		return;


	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetScissor(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetCullMode(Context, CullMode::Back);

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


	Fracture::RenderCommands::UseProgram(Context, mShader->Handle);

	for (auto& batches : Context->mBatches)
	{
		if (batches.second.empty())
			continue;
		
		if (!AssetManager::Instance()->IsMaterialLoaded(batches.first))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(batches.first);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(batches.first);

		if (!material->DepthWrite)
			continue;


		Fracture::RenderCommands::BindMaterial(Context, mShader.get(), material.get());

		if (batches.second.size())
		{
			//Set Shader
			for (auto batch : batches.second)
			{
				const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);

				Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);

				std::vector<std::shared_ptr<MeshDrawCall>> drawcalls = batch.second->OpaqueDrawCalls;
				if (drawcalls.size())
				{
					DrawElementsInstancedBaseVertex cmd;
					cmd.basevertex = drawcalls[0]->basevertex;
					cmd.instancecount = batch.second->Transforms.size();
					cmd.indices = drawcalls[0]->SizeOfindices;
					cmd.count = drawcalls[0]->IndexCount;
					Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
				}

				Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
			}

		}

		/*
		if (material->IsTranslucent)
		{
			RenderCommands::Enable(Context, Fracture::GLCapability::Blending);
			RenderCommands::BlendFunction(Context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);
		}

		if (batches.second.size())
		{
			//Set Shader
			for (auto batch : batches.second)
			{
				const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);

				Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);

				std::vector<std::shared_ptr<MeshDrawCall>> drawcalls = batch.second->TransparentDrawCalls;
				if (drawcalls.size())
				{
					DrawElementsInstancedBaseVertex cmd;
					cmd.basevertex = drawcalls[0]->basevertex;
					cmd.instancecount = batch.second->Transforms.size();
					cmd.indices = drawcalls[0]->SizeOfindices;
					cmd.count = drawcalls[0]->IndexCount;
					Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
				}

				Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
			}

		}

		if (material->IsTranslucent)
			RenderCommands::Disable(Context, Fracture::GLCapability::Blending);		
		*/
	}

	RenderCommands::ReleaseRenderTarget(Context);
	//RenderCommands::Disable(Context, Fracture::GLCapability::StencilTest);
	RenderCommands::SetColorMask(Context, 1, 1, 1, 1);

}
