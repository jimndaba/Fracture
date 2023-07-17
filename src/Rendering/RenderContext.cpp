#include "FracturePCH.h"
#include "RenderContext.h"
#include "GraphicsDevice.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"
#include "Rendering/Material.h"
#include "Mesh.h"

Fracture::RenderContext::RenderContext(RenderContextFlags flags):
	Flags(flags)
{
	Flags = RenderContextFlag_SortOrderBackToFront;
}


void Fracture::RenderContext::BeginState(const SortKey& key)
{
	KeyStack.push(key);
}

void Fracture::RenderContext::EndState()
{
	KeyStack.pop();
}

void Fracture::RenderContext::BeginScene()
{
	ResetBatches();

	for (auto bactcheslist : mBatches)
	{
		for (auto batch : bactcheslist.second)
		{
			batch.second->clear();
		}
	}

	const auto& components = SceneManager::GetAllComponents<MeshComponent>();
	for (const auto& meshcomponent : components)
	{
		if (AssetManager::Instance()->IsMeshLoaded(meshcomponent->Mesh))
		{
			for (const auto& material_id : meshcomponent->Materials)
			{
				const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(meshcomponent->Mesh);
				const auto& transform = SceneManager::GetComponent<TransformComponent>(meshcomponent->GetID());
				auto entity_id = meshcomponent->GetID();
				AddToBatch(material_id, mesh.get(),transform->WorldTransform, entity_id);
			}
		}
	}

	const auto& prefabInstancecomponents = SceneManager::GetAllComponents<PrefabInstanceComponent>();
	for (const auto& prefab : prefabInstancecomponents)
	{
		if (AssetManager::Instance()->IsMeshLoaded(prefab->Mesh))
		{
			for (const auto& material_id : prefab->Materials)
			{
				const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(prefab->Mesh);
				const auto& transform = SceneManager::GetComponent<TransformComponent>(prefab->GetID());
				AddToBatch(material_id, mesh.get(), transform->WorldTransform, prefab->GetParentPrefabID());
			}
		}
	}

	for (auto batches : mBatches)
	{
		if (batches.second.empty())
			continue;

		for (auto& batch : batches.second)
		{
			const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(batch.first);
	
			Fracture::RenderCommands::BufferSubData<glm::mat4>(this, batch.second->Matrix_Buffer->RenderID, batch.second->Transforms, batch.second->Transforms.size() * sizeof(glm::mat4),0);
					
			Fracture::RenderCommands::BufferSubData<glm::vec4>(this, batch.second->EntityID_Buffer->RenderID, batch.second->EntityIDs, batch.second->EntityIDs.size() * sizeof(glm::vec4), 0);
		}
	}
}

void Fracture::RenderContext::Push(Fracture::Command& cmd)
{
	if (KeyStack.empty())
		return;

	cmd.Key = KeyStack.top();
	Commands.push_back(cmd);
}

void Fracture::RenderContext::Sort(DepthSortOrder order)
{
	switch (order)
	{
		case DepthSortOrder::Front_To_Back:
		{
			std::sort(Commands.begin(), Commands.end(), [](Command a, Command b) {return a.Key < b.Key; });
			break;
		}
		case DepthSortOrder::Back_To_Front:
		{
			std::sort(Commands.begin(), Commands.end(), [](Command a, Command b) {return a.Key > b.Key; });
			break;
		}
	}
}

void Fracture::RenderContext::Render()
{
	for (const auto& cmd : Commands)
	{
		cmd.fnc();
		GraphicsDevice::Instance()->CHECKGLERRRORS();
	}

	Commands.clear();
	ActiveTextureUnits = 0;
}

void Fracture::RenderContext::AddToBatch(Fracture::UUID materialID, Fracture::StaticMesh* mesh,glm::mat4 transform, UUID Entity)
{
	if (mBatches.find(materialID) == mBatches.end() || mBatches[materialID].find(mesh->ID) == mBatches[materialID].end())
	{
		mBatches[materialID][mesh->ID] = std::make_shared<RenderBatch>();
		VertexArrayCreationInfo info;
		info.Layout =
		{
			{ ShaderDataType::Float3,"aPos",0,true },
			{ ShaderDataType::Float3,"aNormal" ,0,true},
			{ ShaderDataType::Float2,"aUV" ,0,true},
			{ ShaderDataType::Int4,"aEntityID",1 },
			{ ShaderDataType::Mat4,"instanceMatrix",1}
		};
		GraphicsDevice::Instance()->CreateVertexArray(mBatches[materialID][mesh->ID]->VAO, info);
		GraphicsDevice::Instance()->VertexArray_BindAttributes(mBatches[materialID][mesh->ID]->VAO, info);
		
		{
			GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 0, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID, 0);
			GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 1, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID, sizeof(glm::vec3));
			GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 2, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID, sizeof(glm::vec3) * 2);
			
			GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(mBatches[materialID][mesh->ID]->VAO, mesh->EBO_Buffer->RenderID);
		}
		{
			BufferDescription desc;
			desc.bufferType = BufferType::ArrayBuffer;
			desc.size = sizeof(glm::vec4) * 1024;
			desc.usage = BufferUsage::Dynamic;
			desc.Name = "EntityIDBuffer";
			desc.data = nullptr;
			mBatches[materialID][mesh->ID]->EntityID_Buffer = std::make_shared<Buffer>();
			GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EntityID_Buffer.get(), desc);
			GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 3, sizeof(glm::vec4), mBatches[materialID][mesh->ID]->EntityID_Buffer->RenderID);
		}
		{
			BufferDescription desc;
			desc.bufferType = BufferType::ArrayBuffer;
			desc.size = sizeof(glm::mat4) * 1024;
			desc.usage = BufferUsage::Dynamic;
			desc.Name = "MatrixBuffer";
			desc.data = nullptr;
			mBatches[materialID][mesh->ID]->Matrix_Buffer = std::make_shared<Buffer>();
			GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->Matrix_Buffer.get(), desc);
			GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 4, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID, 0);
			//GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 4, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID, 0);
			//GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 5, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID,
			//	((sizeof(float) * 1) * 4));
			//::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 6, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID,
			//	((sizeof(float) * 2) * 4));
			//GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 7, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID,
			//	((sizeof(float) * 3) * 4));

			GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 4, 1);
			//GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 5, 1);
			//GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 6, 1);
			//GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 7, 1);

			
		}

	
	}

	mBatches[materialID][mesh->ID]->Transforms.push_back(transform);
	glm::vec4 color(0);
	uint32_t id = Entity;

	uint8_t r = (id >> 24) & 0xFF; // Red component
	uint8_t g = (id >> 16) & 0xFF; // Green component
	uint8_t b = (id >> 8) & 0xFF;  // Blue component
	uint8_t a = id & 0xFF;         // Alpha component


	color.r = (float)r / 255.0f;
	color.g = (float)g / 255.0f;
	color.b = (float)b / 255.0f;
	color.a = (float)a / 255.0f;

	mBatches[materialID][mesh->ID]->EntityIDs.push_back(color);
}

void Fracture::RenderContext::ResetBatches()
{
	
}


