#include "FracturePCH.h"
#include "ZPrePass.h"

Fracture::ZPrePass::ZPrePass(const std::string& name, RenderContext* context, const ZPrePassDef& info) :IPass(name, context),Properties(info)
{
}

void Fracture::ZPrePass::Setup()
{
	mShader = AssetManager::Instance()->GetShader("ZPrePass");
	mShaderInstanced = AssetManager::Instance()->GetShader("ZPrePassInstanced");
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


	if (Context->mBatches.empty() && Context->OpaqueDrawCalls.empty())
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
		OPTICK_EVENT("Batch");
		if (!AssetManager::Instance()->IsMaterialLoaded(batches.first))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(batches.first);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(batches.first);
		if (!material->DepthWrite)
			continue;

		Shader* current_shader;
		if (material->IsSkinned)
			current_shader = mShaderSkinned.get();
		if (material->IsInstanced)
			current_shader = mShaderInstanced.get();
		else
			current_shader = mShader.get();		
		
		//Set Shader
		for (auto batch : batches.second)
		{
			if (batch.second->Transforms.empty())
			{
				continue;
			}

			Fracture::RenderCommands::UseProgram(Context, current_shader->Handle);
			Fracture::RenderCommands::BindMaterial(Context, current_shader, material.get());
			Fracture::RenderCommands::SetUniform(Context, current_shader, "MaterialIndex", (int)batch.second->GPUMaterialIndex);

			const auto& mesh = AssetManager::GetStaticByIDMesh(batch.first);
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
		Fracture::RenderCommands::ResetTextureUnits(Context, current_shader);
		
	}

	for (auto& drawCall : Context->OpaqueDrawCalls)
	{
		OPTICK_EVENT("DrawCall");
		if (!AssetManager::Instance()->IsMaterialLoaded(drawCall->MaterialID))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(drawCall->MaterialID);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(drawCall->MaterialID);

		Shader* current_shader;
		if (material->IsSkinned)
			current_shader = mShaderSkinned.get();
		else if (material->IsInstanced)
			current_shader = mShaderInstanced.get();
		else 
			current_shader = mShader.get();

		if (!material->DepthWrite)
			continue;
		
	
		Fracture::RenderCommands::UseProgram(Context, current_shader->Handle);
		Fracture::RenderCommands::SetUniform(Context, current_shader, "MaterialIndex", (int)drawCall->GPUMaterialIndex);
		Fracture::RenderCommands::BindMaterial(Context, current_shader, material.get());
		Fracture::RenderCommands::SetUniform(Context, current_shader, "IsAnimated", false);

		if (!material->IsInstanced)
		{
			Fracture::RenderCommands::SetUniform(Context, current_shader, "Model_matrix", drawCall->model);
		}

		if (material->IsSkinned)
		{			
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

		Fracture::RenderCommands::BindVertexArrayObject(Context,drawCall->MeshHandle);
	
		switch (drawCall->CallType)
		{
			case DrawCommandType::DrawElementsInstancedBaseVertex :
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
				Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer, GraphicsDevice::Instance()->GetIndirectBuffer());
				Fracture::RenderCommands::DrawElementsIndirect(Context,nullptr ,Context->IndirectTerrains.size(),0);
				Fracture::RenderCommands::BindBuffer(Context, BufferType::DrawIndirectBuffer,0);
				break;
			}
	
		}
		Fracture::RenderCommands::ResetTextureUnits(Context, current_shader);
		Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
	}

	RenderCommands::ReleaseRenderTarget(Context);
	RenderCommands::SetColorMask(Context, 1, 1, 1, 1);

}
