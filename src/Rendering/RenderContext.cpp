#include "FracturePCH.h"
#include "RenderContext.h"
#include "GraphicsDevice.h"
#include "Assets/AssetManager.h"
#include "World/SceneManager.h"
#include "Rendering/Material.h"
#include "Mesh.h"

void Fracture::RenderContext::BeginState(const SortKey& key)
{
	KeyStack.push(key);
}

void Fracture::RenderContext::EndState()
{
	KeyStack.pop();
}

void Fracture::RenderContext::Begin()
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
				AddToBatch(material_id, mesh.get(),transform->WorldTransform, meshcomponent->GetID());
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
			Fracture::RenderCommands::MapDataTobuffer<glm::mat4>(this, batch.second->Matrix_Buffer->RenderID, batch.second->Transforms, batch.second->Transforms.size() * sizeof(glm::mat4));

			Fracture::RenderCommands::MapDataTobuffer<glm::vec4>(this, batch.second->EntityID_Buffer->RenderID, batch.second->EntityIDs, batch.second->EntityIDs.size() * sizeof(glm::vec4));
		}
	}



	/*
	for (const auto& meshcomponent : components)
	{
		if (AssetManager::Instance()->IsMeshLoaded(meshcomponent->Mesh))
		{						
			for (const auto& material_id : meshcomponent->Materials)
			{
				const auto& transform = SceneManager::GetComponent<TransformComponent>(meshcomponent->GetID());



				Renderable_batch[material_id][meshcomponent->Mesh].push_back(meshcomponent->entity);			
			}

			const auto& transform = SceneManager::GetComponent<TransformComponent>(meshcomponent->GetID());
			MeshTransformBatch[meshcomponent->Mesh].push_back(transform->WorldTransform);

			glm::vec4 color(0);

			//color.r = (meshcomponent->GetID() >> 0)  / 255.0f;
			//color.g = (meshcomponent->GetID() >> 8) / 255.0f;
			//color.b = (meshcomponent->GetID() >> 16) / 255.0f;

			uint32_t id = meshcomponent->GetID();
			//r = (id & 0x000000FF) >> 0;
			//g = (id & 0x0000FF00) >> 8;
			//b = (id & 0x00FF0000) >> 16;
			//a = 255;

			uint8_t r = (id >> 24) & 0xFF; // Red component
			uint8_t g = (id >> 16) & 0xFF; // Green component
			uint8_t b = (id >> 8) & 0xFF;  // Blue component
			uint8_t a = id & 0xFF;         // Alpha component


			color.r = (float)r / 255.0f;
			color.g = (float)g / 255.0f;
			color.b = (float)b / 255.0f;
			color.a = (float)a / 255.0f;

			EntityIndeces[meshcomponent->Mesh].push_back(color);
		
		}
	}

	for (auto& batch : MeshTransformBatch)
	{
		if (batch.second.empty())
			continue;

		const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(batch.first);
		Fracture::RenderCommands::MapDataTobuffer<glm::mat4>(this, mesh->Matrix_Buffer->RenderID, batch.second, batch.second.size() * sizeof(glm::mat4));
		Fracture::RenderCommands::MapDataTobuffer<glm::vec4>(this, mesh->EntityID_Buffer->RenderID, EntityIndeces[batch.first], EntityIndeces[batch.first].size() * sizeof(glm::vec4));
		
	}
	*/
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
}

void Fracture::RenderContext::AddToBatch(Fracture::UUID materialID, Fracture::StaticMesh* mesh,glm::mat4 transform, UUID Entity)
{
	if (mBatches.find(materialID) == mBatches.end())
	{
		
			mBatches[materialID][mesh->ID] = std::make_shared<RenderBatch>();
			VertexArrayCreationInfo info;
			info.Layout =
			{
				{ ShaderDataType::Float3,"aPos",0,true },
				{ ShaderDataType::Float3,"aNormal" ,0,true},
				{ ShaderDataType::Float2,"aUV" ,0,true},
				{ ShaderDataType::Mat4, "instanceMatrix",1 },
				{ ShaderDataType::Int4,"aEntityID",1 }
			};
			GraphicsDevice::Instance()->CreateVertexArray(mBatches[materialID][mesh->ID]->VAO, info);

			{
				BufferDescription desc;
				desc.data = mesh->mVerticies.data();
				desc.bufferType = BufferType::ArrayBuffer;
				desc.size = sizeof(mesh->mVerticies[0]) * mesh->mVerticies.size();
				desc.usage = BufferUsage::Static;
				desc.Name = "Verticies";
				mBatches[materialID][mesh->ID]->VBO_Buffer = std::make_shared<Buffer>();
				GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->VBO_Buffer.get(), desc);
				GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 0, sizeof(mesh->mVerticies[0]), mBatches[materialID][mesh->ID]->VBO_Buffer->RenderID);
			}
			{
				BufferDescription desc;
				desc.bufferType = BufferType::ArrayBuffer;
				desc.size = sizeof(glm::vec4) * 1024;
				desc.usage = BufferUsage::Static;
				desc.Name = "EntityIDBuffer";
				desc.data = nullptr;
				mBatches[materialID][mesh->ID]->EntityID_Buffer = std::make_shared<Buffer>();
				GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EntityID_Buffer.get(), desc);
				GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 7, sizeof(glm::vec4), mBatches[materialID][mesh->ID]->EntityID_Buffer->RenderID);
			}
			{
				BufferDescription desc;
				desc.bufferType = BufferType::ArrayBuffer;
				desc.size = sizeof(glm::mat4) * 1024;
				desc.usage = BufferUsage::Static;
				desc.Name = "MatrixBuffer";
				desc.data = nullptr;
				mBatches[materialID][mesh->ID]->Matrix_Buffer = std::make_shared<Buffer>();
				GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->Matrix_Buffer.get(), desc);
				GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 3, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID);
			}
			{
				BufferDescription desc;
				desc.data = mesh->Indices.data();
				desc.bufferType = BufferType::ElementArrayBuffer;
				desc.size = sizeof(mesh->Indices[0]) * mesh->Indices.size();
				desc.usage = BufferUsage::Static;
				desc.Name = "IndexBuffer";
				mBatches[materialID][mesh->ID]->EBO_Buffer = std::make_shared<Buffer>();
				GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EBO_Buffer.get(), desc);
				GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(mBatches[materialID][mesh->ID]->VAO, mesh->EBO_Buffer->RenderID);
			}
			GraphicsDevice::Instance()->VertexArray_BindAttributes(mBatches[materialID][mesh->ID]->VAO, info);


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
	else
	{
		if (mBatches[materialID].find(mesh->ID) == mBatches[materialID].end())
		{

			mBatches[materialID][mesh->ID] = std::make_shared<RenderBatch>();
			VertexArrayCreationInfo info;
			info.Layout =
			{
				{ ShaderDataType::Float3,"aPos",0,true },
				{ ShaderDataType::Float3,"aNormal" ,0,true},
				{ ShaderDataType::Float2,"aUV" ,0,true},
				{ ShaderDataType::Mat4, "instanceMatrix",1 },
				{ ShaderDataType::Int4,"aEntityID",1 }
			};
			GraphicsDevice::Instance()->CreateVertexArray(mBatches[materialID][mesh->ID]->VAO, info);

			{
				BufferDescription desc;
				desc.data = mesh->mVerticies.data();
				desc.bufferType = BufferType::ArrayBuffer;
				desc.size = sizeof(mesh->mVerticies[0]) * mesh->mVerticies.size();
				desc.usage = BufferUsage::Static;
				desc.Name = "Verticies";
				mBatches[materialID][mesh->ID]->VBO_Buffer = std::make_shared<Buffer>();
				GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->VBO_Buffer.get(), desc);
				GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 0, sizeof(mesh->mVerticies[0]), mBatches[materialID][mesh->ID]->VBO_Buffer->RenderID);
			}
			{
				BufferDescription desc;
				desc.bufferType = BufferType::ArrayBuffer;
				desc.size = sizeof(glm::vec4) * 1024;
				desc.usage = BufferUsage::Static;
				desc.Name = "EntityIDBuffer";
				desc.data = nullptr;
				mBatches[materialID][mesh->ID]->EntityID_Buffer = std::make_shared<Buffer>();
				GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EntityID_Buffer.get(), desc);
				GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 7, sizeof(glm::vec4), mBatches[materialID][mesh->ID]->EntityID_Buffer->RenderID);
			}
			{
				BufferDescription desc;
				desc.bufferType = BufferType::ArrayBuffer;
				desc.size = sizeof(glm::mat4) * 1024;
				desc.usage = BufferUsage::Static;
				desc.Name = "MatrixBuffer";
				desc.data = nullptr;
				mBatches[materialID][mesh->ID]->Matrix_Buffer = std::make_shared<Buffer>();
				GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->Matrix_Buffer.get(), desc);
				GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 3, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID);
			}
			{
				BufferDescription desc;
				desc.data = mesh->Indices.data();
				desc.bufferType = BufferType::ElementArrayBuffer;
				desc.size = sizeof(mesh->Indices[0]) * mesh->Indices.size();
				desc.usage = BufferUsage::Static;
				desc.Name = "IndexBuffer";
				mBatches[materialID][mesh->ID]->EBO_Buffer = std::make_shared<Buffer>();
				GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EBO_Buffer.get(), desc);
				GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(mBatches[materialID][mesh->ID]->VAO, mesh->EBO_Buffer->RenderID);
			}
			GraphicsDevice::Instance()->VertexArray_BindAttributes(mBatches[materialID][mesh->ID]->VAO, info);
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
}

void Fracture::RenderContext::ResetBatches()
{
	
}
