#include "FracturePCH.h"
#include "ZPrePass.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Animation/AnimationSystem.h"

Fracture::ZPrePass::ZPrePass(const std::string& name, RenderContext* context, const ZPrePassDef& info) :IPass(name, context),Properties(info)
{
}

void Fracture::ZPrePass::Setup()
{
	mShader = AssetManager::Instance()->GetShader("ZPrePass");
	mShaderSkinned = AssetManager::Instance()->GetShader("ZPrePassSkinned");
}

void Fracture::ZPrePass::Execute()
{
	OPTICK_EVENT();
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalColour);

	if (!global_color)
		return;


	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, GraphicsDevice::Instance()->Viewport_Width, GraphicsDevice::Instance()->Viewport_Height, 0, 0);
	RenderCommands::SetScissor(Context, GraphicsDevice::Instance()->Viewport_Width, GraphicsDevice::Instance()->Viewport_Height, 0, 0);
	RenderCommands::SetCullMode(Context, CullMode::None);

	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Less);		


	if (Context->mBatches.empty() && Context->OpaqueDrawCalls.empty() && Context->TransparentDrawCalls.empty())
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
		
		if (!AssetManager::Instance()->IsMaterialLoaded(batches.first))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(batches.first);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(batches.first);
		if (!material->DepthWrite)
			continue;
		
		if (material->IsSkinned)
			Fracture::RenderCommands::UseProgram(Context, mShaderSkinned->Handle);
		else
			Fracture::RenderCommands::UseProgram(Context, mShader->Handle);

		Fracture::RenderCommands::BindMaterial(Context, mShader.get(), material.get());
		Fracture::RenderCommands::SetUniform(Context, mShader.get(), "IsAffectedByWind", material->IsAffectedByWind);
		

		//Set Shader
		for (auto batch : batches.second)
		{
			Fracture::RenderCommands::SetUniform(Context, mShader.get(), "MaterialIndex", (int)batch.second->GPUMaterialIndex);
			const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);
			Fracture::RenderCommands::BindVertexArrayObject(Context, batch.second->VAO);
			if (batch.second->OpaqueDrawCalls.size())
			{
				DrawElementsInstancedBaseVertex cmd;
				cmd.mode = batch.second->OpaqueDrawCalls[0]->DrawCallPrimitive;
				cmd.basevertex = batch.second->OpaqueDrawCalls[0]->basevertex;
				cmd.instancecount = batch.second->Transforms.size();
				cmd.indices = batch.second->OpaqueDrawCalls[0]->SizeOfindices;
				cmd.count = batch.second->OpaqueDrawCalls[0]->IndexCount;
				Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
			}
		}
	}

	for (auto& drawCall : Context->OpaqueDrawCalls)
	{
		if (!AssetManager::Instance()->IsMaterialLoaded(drawCall->MaterialID))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(drawCall->MaterialID);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(drawCall->MaterialID);

		if (!material->DepthWrite)
			continue;

		if (material->IsSkinned)
		{
			Fracture::RenderCommands::UseProgram(Context, mShaderSkinned->Handle);
			Fracture::RenderCommands::BindMaterial(Context, mShaderSkinned.get(), material.get());
			Fracture::RenderCommands::SetUniform(Context, mShaderSkinned.get(), "Model_matrix", drawCall->model);
			Fracture::RenderCommands::SetUniform(Context, mShader.get(), "MaterialIndex", (int)drawCall->GPUMaterialIndex);

			if(AnimationSystem::Instance()->mGlobalPoseMatrices.find(drawCall->EntityID) != AnimationSystem::Instance()->mGlobalPoseMatrices.end())
			{
				const auto& poses = AnimationSystem::Instance()->mGlobalPoseMatrices[drawCall->EntityID];
				if (!poses.empty() && AnimationSystem::Instance()->IsPlaying)
				{
					Fracture::RenderCommands::SetUniform(Context, mShaderSkinned.get(), "IsAnimated", true);
					GraphicsDevice::Instance()->UpdateAnimationData(poses);
				}
				else
				{
					Fracture::RenderCommands::SetUniform(Context, mShaderSkinned.get(), "IsAnimated", false);
				}
			}
		}
		else
		{
			Fracture::RenderCommands::UseProgram(Context, mShader->Handle);
			Fracture::RenderCommands::BindMaterial(Context, mShader.get(), material.get());
			Fracture::RenderCommands::SetUniform(Context, mShader.get(), "Model_matrix", drawCall->model);
			Fracture::RenderCommands::SetUniform(Context, mShader.get(), "IsAnimated", false);
			Fracture::RenderCommands::SetUniform(Context, mShader.get(), "MaterialIndex", (int)drawCall->GPUMaterialIndex);
		}


		
		Fracture::RenderCommands::BindVertexArrayObject(Context,drawCall->MeshHandle);

		DrawElementsInstancedBaseVertex cmd;
		cmd.mode = drawCall->DrawCallPrimitive;
		cmd.basevertex = drawCall->basevertex;
		cmd.instancecount = 1;
		cmd.indices = drawCall->SizeOfindices;
		cmd.count = drawCall->IndexCount;
		Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
		Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
	}



	RenderCommands::ReleaseRenderTarget(Context);
	RenderCommands::SetColorMask(Context, 1, 1, 1, 1);

}
