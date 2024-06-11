#include "FracturePCH.h"
#include "OutlinePass.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Animation/AnimationSystem.h"

Fracture::OutlinePass::OutlinePass(const std::string& name, RenderContext* context, const OutlinePassDef& info) :
	IPass(name,context)
{
}

void Fracture::OutlinePass::Setup()
{
	mZShader = AssetManager::Instance()->GetShader("ZPrePassSkinned");
	mOShader = AssetManager::Instance()->GetShader("Outline");
}

void Fracture::OutlinePass::Execute()
{
	
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget(Fracture::GlobalRenderTargets::GlobalOutline);

	if (!global_color)
		return;



	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::ClearColor(Context,Colour::Black);
	RenderCommands::ClearTarget(Context, (uint32_t)ClearBufferBit::Color | (uint32_t)ClearBufferBit::Depth | (uint32_t)ClearBufferBit::Stencil);

	if (Context->OutlineDrawCalls.empty())
	{

		RenderCommands::ReleaseRenderTarget(Context);
		RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
		RenderCommands::Disable(Context, Fracture::GLCapability::StencilTest);
		return;
	}

	RenderCommands::SetViewport(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetScissor(Context, Context->ContextViewport.Width, Context->ContextViewport.Height, 0, 0);
	RenderCommands::SetCullMode(Context, CullMode::None);

	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::Enable(Context, Fracture::GLCapability::StencilTest);

	RenderCommands::StencilOperation(Context, StencilOp_TestResult::Keep, StencilOp_TestResult::Keep, StencilOp_TestResult::Replace);
	RenderCommands::StencilFunction(Context, StencilFunc::Always,1,0xFF);
	RenderCommands::StencilMask(Context, 0xFF);

	RenderCommands::SetColorMask(Context, 0, 0, 0, 0);

	for (auto& drawcall : Context->OutlineDrawCalls)
	{
		if (!AssetManager::Instance()->IsMaterialLoaded(drawcall->MaterialID))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(drawcall->MaterialID);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(drawcall->MaterialID);
		
		if (material->IsSkinned)
		{
			Fracture::RenderCommands::UseProgram(Context, mZShader->Handle);
			Fracture::RenderCommands::BindMaterial(Context, mZShader.get(), material.get());
			Fracture::RenderCommands::SetUniform(Context, mZShader.get(), "Model_matrix", drawcall->model);

			if (AnimationSystem::Instance()->mGlobalPoseMatrices.find(drawcall->EntityID) != AnimationSystem::Instance()->mGlobalPoseMatrices.end())
			{
				const auto& poses = AnimationSystem::Instance()->mGlobalPoseMatrices[drawcall->EntityID];
				if (!poses.empty())
				{
					Fracture::RenderCommands::SetUniform(Context, mZShader.get(), "IsAnimated", true);
					for (int i = 0; i < poses.size(); i++)
					{
						Fracture::RenderCommands::SetUniform(Context, mZShader.get(), "GlobalPose[" + std::to_string(i) + "]", poses[i]);
					}
				}
				else
				{
					Fracture::RenderCommands::SetUniform(Context, mZShader.get(), "IsAnimated", false);
				}
			}
		}
		else
		{
			Fracture::RenderCommands::UseProgram(Context, mZShader->Handle);
			Fracture::RenderCommands::BindMaterial(Context, mZShader.get(), material.get());
			Fracture::RenderCommands::SetUniform(Context, mZShader.get(), "Model_matrix", drawcall->model);
			Fracture::RenderCommands::SetUniform(Context, mZShader.get(), "IsAnimated", false);
		}

		Fracture::RenderCommands::BindVertexArrayObject(Context, drawcall->MeshHandle);

		DrawElementsInstancedBaseVertex cmd;
		cmd.basevertex = drawcall->basevertex;
		cmd.instancecount = 1;
		cmd.indices = drawcall->SizeOfindices;
		cmd.count = drawcall->IndexCount;
		Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
		Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
	}

		
	RenderCommands::StencilFunction(Context, StencilFunc::NotEqual, 1, 0xFF);
	RenderCommands::StencilMask(Context, 0x00);		
	RenderCommands::SetColorMask(Context, 1, 1, 1, 1);


	for (auto& drawcall : Context->OutlineDrawCalls)
	{
		if (!AssetManager::Instance()->IsMaterialLoaded(drawcall->MaterialID))
		{
			AssetManager::Instance()->AsyncLoadMaterialByID(drawcall->MaterialID);
			continue;
		}

		const auto& material = AssetManager::Instance()->GetMaterialByID(drawcall->MaterialID);
		
		if (material->IsSkinned)
		{
			Fracture::RenderCommands::UseProgram(Context, mOShader->Handle);
			Fracture::RenderCommands::BindMaterial(Context, mOShader.get(), material.get());
			Fracture::RenderCommands::SetUniform(Context, mOShader.get(), "Model_matrix", drawcall->model);

			if (AnimationSystem::Instance()->mGlobalPoseMatrices.find(drawcall->EntityID) != AnimationSystem::Instance()->mGlobalPoseMatrices.end())
			{
				const auto& poses = AnimationSystem::Instance()->mGlobalPoseMatrices[drawcall->EntityID];
				if (!poses.empty())
				{
					Fracture::RenderCommands::SetUniform(Context, mOShader.get(), "IsAnimated", true);
					for (int i = 0; i < poses.size(); i++)
					{
						Fracture::RenderCommands::SetUniform(Context, mOShader.get(), "GlobalPose[" + std::to_string(i) + "]", poses[i]);
					}
				}
				else
				{
					Fracture::RenderCommands::SetUniform(Context, mOShader.get(), "IsAnimated", false);
				}
			}
		}
		else
		{
			Fracture::RenderCommands::UseProgram(Context, mOShader->Handle);
			Fracture::RenderCommands::BindMaterial(Context, mOShader.get(), material.get());
			Fracture::RenderCommands::SetUniform(Context, mOShader.get(), "Model_matrix", drawcall->model);
			Fracture::RenderCommands::SetUniform(Context, mOShader.get(), "IsAnimated", false);
		}

		Fracture::RenderCommands::BindVertexArrayObject(Context, drawcall->MeshHandle);

		DrawElementsInstancedBaseVertex cmd;
		cmd.mode = drawcall->DrawCallPrimitive;
		cmd.basevertex = drawcall->basevertex;
		cmd.instancecount = 1;
		cmd.indices = drawcall->SizeOfindices;
		cmd.count = drawcall->IndexCount;
		Fracture::RenderCommands::DrawElementsInstancedBaseVertex(Context, cmd);
		Fracture::RenderCommands::BindVertexArrayObject(Context, 0);
	}

	RenderCommands::StencilMask(Context, 0xFF);
	RenderCommands::StencilFunction(Context, StencilFunc::Always, 1, 0xFF);

	RenderCommands::ReleaseRenderTarget(Context);
	RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::Disable(Context, Fracture::GLCapability::StencilTest);

}
