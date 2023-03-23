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
	RenderCommands::DepthMask(Context, true);

	const auto& renderables = SceneManager::GetAllComponents<MeshComponent>();
	for (const auto& entity : renderables)
	{
		if (entity)
		{
			if (AssetManager::Instance()->IsMeshLoaded(entity->Mesh))
			{
				const auto& transform = SceneManager::GetComponent<TransformComponent>(entity->GetID());
				// [Shader][Mesh]
				Context->Renderable_batch[mShader->ID][entity->Mesh].push_back(transform->WorldTransform);
			}
		}
	}

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

	RenderCommands::ReleaseRenderTarget(Context);
	RenderCommands::SetColorMask(Context,1, 1, 1, 1);
	RenderCommands::Disable(Context, Fracture::GLCapability::DepthTest);

}
