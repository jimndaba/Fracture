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
	const auto& global_color = GraphicsDevice::Instance()->GetGlobalRenderTarget("Global_ColorBuffer");

	if (!global_color)
		return;

	int clearValue = 1;


	RenderCommands::SetRenderTarget(Context, global_color);
	RenderCommands::SetViewport(Context, 1920, 1080, 0, 0);
	RenderCommands::SetScissor(Context, 1920, 1080, 0, 0);
	RenderCommands::SetCullMode(Context, CullMode::Back);	
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

			
				int r = ((uint32_t)entity.first & 0x000000FF) >> 0;
				int g = ((uint32_t)entity.first & 0x0000FF00) >> 8;
				int b = ((uint32_t)entity.first & 0x00FF0000) >> 16;

				RenderCommands::SetUniform(Context,shader.get(),"PickingID", glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f));




				if (mesh->mMaterials.size())
					Fracture::RenderCommands::BindMaterial(Context, shader.get(), AssetManager::Instance()->GetMaterialByID(mesh->mMaterials[sub.MaterialIndex]).get());

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

	RenderCommands::ReleaseRenderTarget(Context);
	


}

void Fracture::ForwardPass::PickPass()
{


	glEnable(GL_DITHER);


}
