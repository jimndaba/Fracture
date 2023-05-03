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
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_ColorBuffer");

	if (!global_color)
		return;


	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, 1920, 1080, 0, 0);
	RenderCommands::SetScissor(Context, 1920, 1080, 0, 0);
	RenderCommands::SetCullMode(Context, CullMode::Back);

	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Less);
	//RenderCommands::DepthMask(Context, true);

	//RenderCommands::SetColorMask(Context, 0, 0, 0, 0);
	
	//Issue out Batch Commands.

	if (Context->Renderable_batch.empty())
	{
		RenderCommands::ReleaseRenderTarget(Context);
		RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
		RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
		return;
	}

	for (auto& batch : Context->Renderable_batch)
	{
		if (batch.second.empty())
			continue;

		Fracture::RenderCommands::UseProgram(Context, mShader->Handle);
		//Set Shader
		for (auto entity : batch.second)
		{
			const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(entity.first);

			Fracture::RenderCommands::BindVertexArrayObject(Context, mesh->VAO);

			for (const auto& sub : mesh->SubMeshes)
			{
				if (!AssetManager::Instance()->IsMaterialLoaded(mesh->mMaterials[sub.MaterialIndex]))
				{
					if (mesh->mMaterials.size())
						AssetManager::Instance()->AsyncLoadMaterialByID(mesh->mMaterials[sub.MaterialIndex]);
				}

				if (mesh->mMaterials.size())
					Fracture::RenderCommands::BindMaterial(Context, mShader.get(), AssetManager::Instance()->GetMaterialByID(mesh->mMaterials[sub.MaterialIndex]).get());

				DrawElementsInstancedBaseVertex cmd;
				cmd.basevertex = sub.BaseVertex;
				cmd.instancecount = entity.second.size();
				cmd.indices = (void*)(sizeof(unsigned int) * sub.BaseIndex);
				cmd.count = sub.IndexCount;
				Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
			}
			Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
		}
	}

	/*
	for (auto& batch : Context->Renderable_batch)
	{
		if (batch.second.empty())
			continue;

		Fracture::RenderCommands::UseProgram(Context, mShader->Handle);

		for (auto entity : batch.second)
		{
			const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(entity.first);
			Fracture::RenderCommands::BindVertexArrayObject(Context, mesh->VAO);	

			for (const auto& sub : mesh->SubMeshes)
			{
				DrawElementsInstancedBaseVertex cmd;
				cmd.basevertex = sub.BaseVertex;
				cmd.instancecount = entity.second.size();
				cmd.indices = (void*)(sizeof(unsigned int) * sub.BaseIndex);
				cmd.count = sub.IndexCount;
				Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
			}
		}
	}
	*/
	//RenderCommands::SetColorMask(Context, 1, 1, 1, 1);
	RenderCommands::ReleaseRenderTarget(Context);
	RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);

}
