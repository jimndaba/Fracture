#include "FracturePCH.h"
#include "ZPrePass.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"
#include "Rendering/Mesh.h"

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
	RenderCommands::SetViewport(Context, 1920, 1080, 0, 0);
	RenderCommands::SetScissor(Context, 1920, 1080, 0, 0);
	RenderCommands::SetCullMode(Context, CullMode::None);

	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Less);	

	if (Context->mBatches.empty())
	{
		RenderCommands::ReleaseRenderTarget(Context);
		RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
		RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
		return;
	}

	for (auto& batches : Context->mBatches)
	{
		if (batches.second.empty())
			continue;

		Fracture::RenderCommands::UseProgram(Context, mShader->Handle);

		if (!AssetManager::Instance()->IsMaterialLoaded(batches.first))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(batches.first);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(batches.first);
		Fracture::RenderCommands::BindMaterial(Context, mShader.get(), material.get());

		//Set Shader
		for (auto batch: batches.second)
		{
			const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(batch.first);
			Fracture::RenderCommands::BindVertexArrayObject(Context,batch.second->VAO);

			for (const auto& sub : mesh->SubMeshes)
			{
				DrawElementsInstancedBaseVertex cmd;
				cmd.basevertex = sub.BaseVertex;
				cmd.instancecount = batch.second->Transforms.size();
				cmd.indices = (void*)(sizeof(unsigned int) * sub.BaseIndex);
				cmd.count = sub.IndexCount;
				Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
			}
			Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
		}
	}

	RenderCommands::ReleaseRenderTarget(Context);
	RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);

}
