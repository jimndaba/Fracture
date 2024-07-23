#include "FracturePCH.h"
#include "ForwardPass.h"


Fracture::ForwardPass::ForwardPass(const std::string& name, RenderContext* context, const ForwardPassDef& info):IPass(name,context),definition(info)
{
}

void Fracture::ForwardPass::Setup()
{


}

void Fracture::ForwardPass::Execute()
{
	OPTICK_EVENT();
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);

	if (!global_color)
		return;
	
	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, GraphicsDevice::Instance()->Viewport_Width, GraphicsDevice::Instance()->Viewport_Height, 0, 0);
	RenderCommands::SetScissor(Context, GraphicsDevice::Instance()->Viewport_Width, GraphicsDevice::Instance()->Viewport_Height, 0, 0);
	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Equal);
	RenderCommands::Enable(Context, Fracture::GLCapability::FaceCulling);

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
		if (material->IsTranslucent)
		{
			continue;
		}

		const auto& shader = AssetManager::Instance()->GetShaderByID(material->Shader);
		Fracture::RenderCommands::UseProgram(Context, shader->Handle);

		RenderCommands::SetCullMode(Context, material->cullmode);

		if (GraphicsDevice::Instance()->RenderSettings.SSAO_Enabled)
		{
			const auto& global_SSAO = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSSAO);
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aGlobalAO", global_SSAO->ColorAttachments[0]->Handle, (int)TEXTURESLOT::GlobalAO);
		}
		{
			const auto& global_Shadows = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows);
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aShadowMap", global_Shadows->DepthStencilAttachment->Handle, (int)TEXTURESLOT::DirectShadows);

			const auto& spot_Shadows = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSpotShadows);
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aSpotMap", spot_Shadows->DepthStencilAttachment->Handle, (int)TEXTURESLOT::GlobalSpots);


			std::vector<glm::mat4> mSpotMatricies;
			mSpotMatricies.resize(10);
			const auto& lightlist = SceneManager::GetAllComponents<SpotlightComponent>();
			for (int i = 0; i < lightlist.size(); i++)
			{
				if (i > mSpotMatricies.size())
					continue;

				const auto& transform = SceneManager::GetComponent<TransformComponent>(lightlist[i]->GetID());

				float outerConeAngleRadians = glm::radians(lightlist[i]->OutCutoff);
				// Calculate the full field of view
				float fov = 2.0f * outerConeAngleRadians;

				glm::mat4 projection = glm::perspective(fov, 1.0f, 0.1f, 100.0f);


				glm::vec3 defaultDirection = glm::vec3(0.0f, 0.0f, -1.0f);
				glm::vec3 spotlightDirection = glm::normalize(glm::rotate(transform->Rotation, defaultDirection));

				glm::mat4 view = glm::lookAt(transform->Position, transform->Position + spotlightDirection, glm::vec3(0, 1, 0));
				mSpotMatricies[i] = projection * view;
			}

			for (int i = 0; i < mSpotMatricies.size(); i++)
			{
				Fracture::RenderCommands::SetUniform(Context, shader.get(), "SpotMatrices[" + std::to_string(i) + "]", mSpotMatricies[i]);
			}
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

		if (batches.second.size())
		{
			//Set Shader
			for (auto batch : batches.second)
			{
				if (batch.second->Transforms.empty())
				{
					continue;
				}
				Fracture::RenderCommands::BindMaterial(Context, shader.get(), material.get());
				Fracture::RenderCommands::SetUniform(Context, shader.get(), "MaterialIndex", (int)batch.second->GPUMaterialIndex);

				Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);
				DrawElementsInstancedBaseVertex cmd;
				cmd.mode = batch.second->DrawCallPrimitive;
				cmd.basevertex = batch.second->basevertex;
				cmd.instancecount = batch.second->Transforms.size();
				cmd.indices = batch.second->SizeOfindices;
				cmd.count = batch.second->IndexCount;
				Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
				Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
			}

		}
		Fracture::RenderCommands::ResetTextureUnits(Context, shader.get());
	}


	for (auto& drawCall : Context->OpaqueDrawCalls)
	{
		if (!AssetManager::Instance()->IsMaterialLoaded(drawCall->MaterialID))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(drawCall->MaterialID);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(drawCall->MaterialID);
		if (material->IsTranslucent)
		{
			continue;
		}

		const auto& shader = AssetManager::Instance()->GetShaderByID(material->Shader);

		Fracture::RenderCommands::UseProgram(Context, shader->Handle);
		RenderCommands::SetCullMode(Context, material->cullmode);

		Fracture::RenderCommands::SetUniform(Context, shader.get(), "EntityID", drawCall->IDColor);
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "Model_matrix", drawCall->model);
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "MaterialIndex", (int)drawCall->GPUMaterialIndex);
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "IsEditingTerrain", GraphicsDevice::Instance()->RenderSettings.IsEditingTerrain);
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "MousePosRadius", GraphicsDevice::Instance()->RenderSettings.MousePos_BrushRadius);
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "BrushFalloff", GraphicsDevice::Instance()->RenderSettings.BrushFallOff);
		Fracture::RenderCommands::BindMaterial(Context, shader.get(), material.get());

		if (material->IsSkinned)
		{
			if (AnimationSystem::Instance()->mGlobalPoseMatrices.find(drawCall->EntityID) != AnimationSystem::Instance()->mGlobalPoseMatrices.end())
			{
				const auto& poses = AnimationSystem::Instance()->mGlobalPoseMatrices[drawCall->EntityID];
				if (!poses.empty() && AnimationSystem::Instance()->IsPlaying)
				{
					Fracture::RenderCommands::SetUniform(Context, shader.get(), "IsAnimated", true);
					GraphicsDevice::Instance()->UpdateAnimationData(poses);
				}
				else
				{
					Fracture::RenderCommands::SetUniform(Context, shader.get(), "IsAnimated", false);
				}
			}
			else
			{
				Fracture::RenderCommands::SetUniform(Context, shader.get(), "IsAnimated", false);
			}
		}

		if (GraphicsDevice::Instance()->RenderSettings.SSAO_Enabled)
		{
			const auto& global_SSAO = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSSAO);
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aGlobalAO", global_SSAO->ColorAttachments[0]->Handle, (int)TEXTURESLOT::GlobalAO);
		}
		{
			const auto& global_Shadows = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalDirectShadows);
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aShadowMap", global_Shadows->DepthStencilAttachment->Handle, (int)TEXTURESLOT::DirectShadows);

			const auto& spot_Shadows = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalSpotShadows);
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "aSpotMap", spot_Shadows->DepthStencilAttachment->Handle, (int)TEXTURESLOT::GlobalSpots);


			std::vector<glm::mat4> mSpotMatricies;
			mSpotMatricies.resize(10);
			const auto& lightlist = SceneManager::GetAllComponents<SpotlightComponent>();
			for (int i = 0; i < lightlist.size(); i++)
			{
				if (i > mSpotMatricies.size())
					continue;

				const auto& transform = SceneManager::GetComponent<TransformComponent>(lightlist[i]->GetID());

				float outerConeAngleRadians = glm::radians(lightlist[i]->OutCutoff);
				// Calculate the full field of view
				float fov = 2.0f * outerConeAngleRadians;

				glm::mat4 projection = glm::perspective(fov, 1.0f, 0.1f, 100.0f);


				glm::vec3 defaultDirection = glm::vec3(0.0f, 0.0f, -1.0f);
				glm::vec3 spotlightDirection = glm::normalize(glm::rotate(transform->Rotation, defaultDirection));

				glm::mat4 view = glm::lookAt(transform->Position, transform->Position + spotlightDirection, glm::vec3(0, 1, 0));
				mSpotMatricies[i] = projection * view;
			}

			for (int i = 0; i < mSpotMatricies.size(); i++)
			{
				Fracture::RenderCommands::SetUniform(Context, shader.get(), "SpotMatrices[" + std::to_string(i) + "]", mSpotMatricies[i]);
			}
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

		//const auto& probes = GraphicsDevice::Instance()->GetLightProbeIrradiance();
		//Fracture::RenderCommands::SetTexture(Context, shader.get(), "aReflections", probes, (int)TEXTURESLOT::Reflections);
		//Fracture::RenderCommands::SetUniform(Context, shader.get(), "_ReflectionFlag", material->IsReflective);

		Fracture::RenderCommands::BindVertexArrayObject(Context, drawCall->MeshHandle);
		switch (drawCall->CallType)
		{
		case DrawCommandType::DrawElementsInstancedBaseVertex:
		{
			DrawElementsInstancedBaseVertex cmd;
			cmd.mode = drawCall->DrawCallPrimitive;
			cmd.basevertex = drawCall->basevertex;
			cmd.instancecount = 1;
			cmd.indices = drawCall->SizeOfindices;
			cmd.count = drawCall->IndexCount;
			Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
			break;
		}
		case DrawCommandType::MultiDrawElementsIndirect:
		{
			DrawElementsInstancedBaseVertex cmd;
			Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer, GraphicsDevice::Instance()->GetIndirectBuffer());
			Fracture::RenderCommands::DrawElementsIndirect(Context, nullptr, Context->IndirectTerrains.size(), 0);
			Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer, 0);
			break;
		}

		case DrawCommandType::DrawArrys:
		{
			DrawArray cmd;
			cmd.count = drawCall->IndexCount;
			cmd.first = drawCall->baseIndex;
			cmd.mode = drawCall->DrawCallPrimitive;
			Fracture::RenderCommands::DrawArray(Context, cmd);
			break;
		}

		}
		Fracture::RenderCommands::ResetTextureUnits(Context, shader.get());
		Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
	}

	Fracture::RenderCommands::UseProgram(Context,0);


	//Fracture::RenderCommands::BlitFramebuffer(Context, Fracture::GlobalRenderTargets::GlobalColour, Fracture::GlobalRenderTargets::GlobalGrabs);
	
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::LEqual);
	Fracture::DebugRenderer::OnRenderSceneBillboards(Context);

	
	RenderCommands::Enable(Context, Fracture::GLCapability::Blending);
	RenderCommands::BlendFunction(Context, Fracture::BlendFunc::SrcAlpha, Fracture::BlendFunc::OneMinusSrcAlpha);

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
		if (!material->IsTranslucent)
		{
			continue;
		}

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
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "irradianceMap", sky, (int)TEXTURESLOT::Irradiance);

			const auto& prefilter = GraphicsDevice::Instance()->GetSpecularBRDFMap(global_probes[0]->GetID());
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "prefilterMap", prefilter, (int)TEXTURESLOT::Prefilter);

			const auto& brdflut = GraphicsDevice::Instance()->GetBRDFLUTMap(global_probes[0]->GetID());
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "brdfLUT", brdflut, (int)TEXTURESLOT::BRDF);
		}		
		const auto& probes = GraphicsDevice::Instance()->GetLightProbeIrradiance();
		Fracture::RenderCommands::SetTexture(Context, shader.get(), "aReflections", probes, (int)TEXTURESLOT::Reflections);
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "_ReflectionFlag", material->IsReflective);
		RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Equal);
		

		if (batches.second.size())
		{
			//Set Shader
			for (auto batch : batches.second)
			{
				const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);
				Fracture::RenderCommands::SetUniform(Context, shader.get(), "MaterialIndex", (int)batch.second->GPUMaterialIndex);
				Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);

				//std::vector<std::shared_ptr<MeshDrawCall>> drawcalls = batch.second->TransparentDrawCalls;
				if (batch.second->Transforms.size())
				{
					DrawElementsInstancedBaseVertex cmd;
					cmd.basevertex = batch.second->basevertex;
					cmd.instancecount = batch.second->Transforms.size();
					cmd.indices = batch.second->SizeOfindices;
					cmd.count = batch.second->IndexCount;
					Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
				}

				Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
			}

		}		
		Fracture::RenderCommands::ResetTextureUnits(Context, shader.get());
	}

	for (auto& drawCall : Context->TransparentDrawCalls)
	{

		if (!AssetManager::Instance()->IsMaterialLoaded(drawCall->MaterialID))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(drawCall->MaterialID);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(drawCall->MaterialID);
		if (!material->IsTranslucent)
		{
			continue;
		}

		const auto& shader = AssetManager::Instance()->GetShaderByID(material->Shader);

		Fracture::RenderCommands::UseProgram(Context, shader->Handle);

		Fracture::RenderCommands::BindMaterial(Context, shader.get(), material.get());
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "MaterialIndex", (int)drawCall->GPUMaterialIndex);
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
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "irradianceMap", sky, (int)TEXTURESLOT::Irradiance);

			const auto& prefilter = GraphicsDevice::Instance()->GetSpecularBRDFMap(global_probes[0]->GetID());
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "prefilterMap", prefilter, (int)TEXTURESLOT::Prefilter);

			const auto& brdflut = GraphicsDevice::Instance()->GetBRDFLUTMap(global_probes[0]->GetID());
			Fracture::RenderCommands::SetTexture(Context, shader.get(), "brdfLUT", brdflut, (int)TEXTURESLOT::BRDF);



		}

		const auto& probes = GraphicsDevice::Instance()->GetLightProbeIrradiance();
		Fracture::RenderCommands::SetTexture(Context, shader.get(), "aReflections", probes, (int)TEXTURESLOT::Reflections);
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "_ReflectionFlag", material->IsReflective);
		RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Equal);

		Fracture::RenderCommands::SetUniform(Context, shader.get(), "EntityID", drawCall->IDColor);
		Fracture::RenderCommands::SetUniform(Context, shader.get(), "Model_matrix", drawCall->model);
		
		Fracture::RenderCommands::BindVertexArrayObject(Context, drawCall->MeshHandle);

		switch (drawCall->CallType)
		{
		case DrawCommandType::DrawElementsInstancedBaseVertex:
		{
			DrawElementsInstancedBaseVertex cmd;
			cmd.mode = drawCall->DrawCallPrimitive;
			cmd.basevertex = drawCall->basevertex;
			cmd.instancecount = 1;
			cmd.indices = drawCall->SizeOfindices;
			cmd.count = drawCall->IndexCount;
			Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
			break;
		}
	

		case DrawCommandType::MultiDrawElementsIndirect:
		{
			DrawElementsInstancedBaseVertex cmd;
			Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer, GraphicsDevice::Instance()->GetIndirectBuffer());
			Fracture::RenderCommands::DrawElementsIndirect(Context, Context->IndirectTerrains.data());
			break;
		}

		case DrawCommandType::DrawArrys:
		{
			DrawArray cmd;
			cmd.count = drawCall->IndexCount;
			cmd.first = drawCall->baseIndex;
			cmd.mode = drawCall->DrawCallPrimitive;
			Fracture::RenderCommands::DrawArray(Context, cmd);
			break;
		}
	
		}

		Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
	}

	RenderCommands::Disable(Context, Fracture::GLCapability::Blending);


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
