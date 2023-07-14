#include "FracturePCH.h"
#include "ForwardPass.h"
#include "World/SceneManager.h"
#include "Assets/AssetManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "World/LightProbeSystem.h"

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
	RenderCommands::SetViewport(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetScissor(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::Enable(Context, Fracture::GLCapability::FaceCulling);
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Equal);

	RenderCommands::ClearImage(Context, global_color->ColorAttachments[1]->Handle, 0, &clearValue);
	

	//Issue out Batch Commands.

	

	if (!Context->mBatches.empty())
	{
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
			const auto& shader = AssetManager::Instance()->GetShaderByID(material->Shader);


			Fracture::RenderCommands::UseProgram(Context, shader->Handle);
			Fracture::RenderCommands::BindMaterial(Context, shader.get(), material.get());
			RenderCommands::SetCullMode(Context, material->cullmode);

			if (GraphicsDevice::Instance()->RenderSettings.SSAO_Enabled)
			{
				const auto& global_SSAO = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSSAO);
				Fracture::RenderCommands::SetTexture(Context, shader.get(), "aGlobalAO", global_SSAO->ColorAttachments[0]->Handle, (int)TEXTURESLOT::GlobalAO);
			}
			{
				const auto& global_Shadows = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows);
				Fracture::RenderCommands::SetTexture(Context, shader.get(), "aShadowMap", global_Shadows->DepthStencilAttachment->Handle, (int)TEXTURESLOT::DirectShadows);
			}

			const auto& global_probes = SceneManager::GetAllComponents<LightProbeComponent>();
			if (!global_probes.empty() && global_probes[0]->IsBaked)
			{				
				const auto& sky = GraphicsDevice::Instance()->GetIrradianceMap(global_probes[0]->GetID());
				Fracture::RenderCommands::SetTexture(Context, shader.get(), "irradianceMap", sky, (int)TEXTURESLOT::GlobalIrradiance);
				
			}


			//Set Shader
			for (auto batch : batches.second)
			{
				const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(batch.first);
				Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);

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

			Fracture::RenderCommands::UseProgram(Context, 0);
		}
	}

	for (const auto& skybox : SceneManager::GetAllComponents<SkyboxComponent>())
	{
		const auto& shader = AssetManager::Instance()->GetShader("Skybox");
		Fracture::RenderCommands::UseProgram(Context, shader->Handle);		
		RenderCommands::DepthFunction(Context,DepthFunc::LEqual);
		RenderCommands::Disable(Context,Fracture::GLCapability::FaceCulling);

	
		if (skybox->IsSkyTextureSet)
		{
			const auto& sky = GraphicsDevice::Instance()->GetGlobalTexture("Global_SkyMap");
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "skyMap", sky->Handle, 0);
			GraphicsDevice::Instance()->RenderCaptureCube(Context);

		}
	
	}
	
	RenderCommands::ReleaseRenderTarget(Context);
}

void Fracture::ForwardPass::PickPass()
{


	glEnable(GL_DITHER);


}
