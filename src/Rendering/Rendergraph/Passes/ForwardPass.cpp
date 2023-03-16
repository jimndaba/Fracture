#include "FracturePCH.h"
#include "ForwardPass.h"
#include "World/SceneManager.h"
#include "Assets/AssetManager.h"
#include "Rendering/Mesh.h"

Fracture::ForwardPass::ForwardPass(const std::string& name, RenderContext* context, const ForwardPassDef& info):IPass(name,context),definition(info)
{
}

void Fracture::ForwardPass::Setup()
{


}

void Fracture::ForwardPass::Execute()
{

	Renderable_batch.clear();

	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_ColorBuffer");

	if (!global_color)
		return;


	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, 1920, 1080, 0, 0);
	RenderCommands::SetScissor(Context, 1920, 1080, 0, 0);
	RenderCommands::SetCullMode(Context, CullMode::Back);
	
	RenderCommands::Enable(Context, Fracture::GLCapability::DepthTest);
	RenderCommands::DepthFunction(Context, Fracture::DepthFunc::Less);

	const auto& renderables = SceneManager::GetAllComponents<MeshComponent>();
	for (const auto& entity : renderables)
	{
		if (entity)
		{
			if (AssetManager::Instance()->IsMeshLoaded(entity->Mesh))
			{
				const auto& transform = SceneManager::GetComponent<TransformComponent>(entity->GetID());
				// [Shader][Mesh]
				Renderable_batch[entity->Shader][entity->Mesh].push_back(transform->WorldTransform);
			}
		}
	}

	//Issue out Batch Commands.

	if (Renderable_batch.empty())
	{
		RenderCommands::ReleaseRenderTarget(Context);
		return;
	}

	for (auto& batch : Renderable_batch)
	{
		if (batch.second.empty())
			continue;

		//Set Shader

		const auto& shader = AssetManager::Instance()->GetShaderByID(batch.first);

		Fracture::RenderCommands::UseProgram(Context, shader->Handle);

		for (auto entity : batch.second)
		{
			const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(entity.first);

			Fracture::RenderCommands::MapDataTobuffer<glm::mat4>(Context, mesh->Matrix_Buffer->RenderID, entity.second, entity.second.size() * sizeof(glm::mat4));

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
	


}
