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

	//for (const auto& probe : SceneManager::GetAllComponents<LightProbeComponent>())
	//{
	//	if (probe->ProbeType == LightProbeComponent::LightProbeType::Local)
	//	{
	//		LightProbeSystem s;
	//		s.Bake(Context, probe->GetID());
	//	}
	//}
	

	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetScissor(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::Enable(Context, Fracture::GLCapability::FaceCulling);
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Equal);	

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
			
			if (material->IsTranslucent)
			{
				RenderCommands::Enable(Context, Fracture::GLCapability::Blending);
				RenderCommands::BlendFunction(Context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);
			}

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
				Fracture::RenderCommands::SetTexture(Context, shader.get(), "irradianceMap", sky, (int)TEXTURESLOT::Irradiance);

				const auto& prefilter = GraphicsDevice::Instance()->GetSpecularBRDFMap(global_probes[0]->GetID());
				Fracture::RenderCommands::SetTexture(Context, shader.get(), "prefilterMap", prefilter, (int)TEXTURESLOT::Prefilter);

				const auto& brdflut = GraphicsDevice::Instance()->GetBRDFLUTMap(global_probes[0]->GetID());
				Fracture::RenderCommands::SetTexture(Context, shader.get(), "brdfLUT", brdflut, (int)TEXTURESLOT::BRDF);

				

			}
			if (material->IsReflective)
			{
				
				
			}
			const auto& probes = GraphicsDevice::Instance()->GetLightProbeIrradiance();
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aReflections", probes, (int)TEXTURESLOT::Reflections);
			Fracture::RenderCommands::SetUniform(Context, shader.get(), "_ReflectionFlag", material->IsReflective);

			if (batches.second.size())
			{
				//Set Shader
				for (auto batch : batches.second)
				{
					const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);

					Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);
					
					std::vector<std::shared_ptr<MeshDrawCall>> drawcalls;
					if (material->IsTranslucent)
						drawcalls = batch.second->TransparentDrawCalls;
					else
						drawcalls = batch.second->OpaqueDrawCalls;

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

			Fracture::RenderCommands::ResetTextureUnits(Context,shader.get());			
		}
	
	}


	for (const auto& skybox : SceneManager::GetAllComponents<SkyboxComponent>())
	{
		if (!skybox->IsReady)
			continue;

		const auto& shader = AssetManager::Instance()->GetShader("Skybox");
		Fracture::RenderCommands::UseProgram(Context, shader->Handle);		
		RenderCommands::DepthFunction(Context,DepthFunc::LEqual);
		RenderCommands::Disable(Context,Fracture::GLCapability::FaceCulling);

		if (skybox->IsSkyTextureSet)
		{
			const auto& sky = GraphicsDevice::Instance()->GetGlobalTexture("Global_SkyMap");
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "skyMap", sky->Handle, 0);
		}

		GraphicsDevice::Instance()->RenderCaptureCube(Context);	
	}

	RenderCommands::ReleaseRenderTarget(Context);
}

void Fracture::ForwardPass::PickPass()
{


	glEnable(GL_DITHER);


}
