#include "FracturePCH.h"
#include "ForwardPass.h"
#include "World/SceneManager.h"
#include "Assets/AssetManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

Fracture::ForwardPass::ForwardPass(const std::string& name, RenderContext* context, const ForwardPassDef& info):IPass(name,context),definition(info)
{
}

void Fracture::ForwardPass::Setup()
{


}

void Fracture::ForwardPass::Execute()
{
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);

	if (!global_color)
		return;

	int clearValue = 1;


	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, 1920, 1080, 0, 0);
	RenderCommands::SetScissor(Context, 1920, 1080, 0, 0);
	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Equal);

	RenderCommands::ClearImage(Context, global_color->ColorAttachments[1]->Handle, 0, &clearValue);
	

	//Issue out Batch Commands.

	if (Context->Renderable_batch.empty())
	{
		RenderCommands::ReleaseRenderTarget(Context);
		return;
	}

	for (auto& batch : Context->Renderable_batch)
	{
		if (batch.second.empty())
			continue;
		
		const auto& shader = AssetManager::Instance()->GetShaderByID(batch.first);
		Fracture::RenderCommands::UseProgram(Context, shader->Handle);


		if (GraphicsDevice::Instance()->RenderSettings.SSAO_Enabled)
		{
			const auto& global_SSAO = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSSAO);
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aGlobalAO", global_SSAO->ColorAttachments[0]->Handle, 0);
		}		
		{
			const auto& global_Shadows = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows);
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aShadowMap", global_Shadows->DepthStencilAttachment->Handle, 1);
		}

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
					Fracture::RenderCommands::BindMaterial(Context, shader.get(), AssetManager::Instance()->GetMaterialByID(mesh->mMaterials[sub.MaterialIndex]).get());

				RenderCommands::SetCullMode(Context, AssetManager::Instance()->GetMaterialByID(mesh->mMaterials[sub.MaterialIndex])->cullmode);


				DrawElementsInstancedBaseVertex cmd;
				cmd.basevertex = sub.BaseVertex;
				cmd.instancecount = entity.second.size();
				cmd.indices = (void*)(sizeof(unsigned int) * sub.BaseIndex);
				cmd.count = sub.IndexCount;
				Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);		

				//Fracture::RenderCommands::ResetTextureUnits(Context, shader.get());
			}
			Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
		}

		Fracture::RenderCommands::UseProgram(Context, 0);


	}


	

	RenderCommands::ReleaseRenderTarget(Context);
	


}

void Fracture::ForwardPass::PickPass()
{


	glEnable(GL_DITHER);


}
