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
	OPTICK_EVENT();
	ResetBatches();	
	OpaqueDrawCalls.clear();
	TransparentDrawCalls.clear();
	ShadowDrawCalls.clear();


	const auto& components = SceneManager::GetAllComponents<MeshComponent>();
	for (const auto& meshcomponent : components)
	{
		const auto& transform = SceneManager::GetComponent<TransformComponent>(meshcomponent->GetID());
		if(meshcomponent->meshType == MeshComponent::MeshType::Static)
			AddToBatch(meshcomponent.get(), transform->WorldTransform, meshcomponent->GetID());

		if (meshcomponent->meshType == MeshComponent::MeshType::Skinned)
			AddDrawCall(meshcomponent.get(), transform->WorldTransform, meshcomponent->GetID());
	}

	const auto& prefabInstancecomponents = SceneManager::GetAllComponents<PrefabInstanceComponent>();
	for (const auto& prefab : prefabInstancecomponents)
	{
		const auto& transform = SceneManager::GetComponent<TransformComponent>(prefab->GetID());
		if (prefab->meshType == PrefabInstanceComponent::MeshType::Static)
			AddToBatch(prefab.get(), transform->WorldTransform, prefab->Parent_PrefabID);
	}

	for (auto batches : mBatches)
	{
		if (batches.second.empty())
			continue;

		for (auto& batch : batches.second)
		{
			//const auto& mesh = AssetManager::Instance()->GetStaticByIDMesh(batch.first);
	
			Fracture::RenderCommands::BufferSubData<glm::mat4>(this, batch.second->Matrix_Buffer->RenderID, batch.second->Transforms, batch.second->Transforms.size() * sizeof(glm::mat4),0);
					
			Fracture::RenderCommands::BufferSubData<glm::vec4>(this, batch.second->EntityID_Buffer->RenderID, batch.second->EntityIDs, batch.second->EntityIDs.size() * sizeof(glm::vec4), 0);
		}
	}
}

void Fracture::RenderContext::EndScene()
{
	OutlineDrawCalls.clear();
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
	OPTICK_EVENT();
	currentIndex = 0;
	for (const auto& cmd : Commands)
	{
		if(cmd.fnc)
			cmd.fnc();
		GraphicsDevice::Instance()->CHECKGLERRRORS();
		currentIndex++;
	}

	Commands.clear();
	ActiveTextureUnits = 0;
}

void Fracture::RenderContext::AddToBatch(MeshComponent* meshcomponent,glm::mat4 transform, UUID entity)
{
	OPTICK_EVENT();
	if (!AssetManager::Instance()->IsMeshLoaded(meshcomponent->Mesh))
		return;

	const auto& mesh = AssetManager::GetStaticByIDMesh(meshcomponent->Mesh);

	for (const auto& submesh : mesh->SubMeshes)
	{
		int material_index = (int)submesh.MaterialIndex;
		auto materialID = meshcomponent->Materials[material_index];
		if (mBatches.find(materialID) == mBatches.end() || mBatches[materialID].find(mesh->ID) == mBatches[materialID].end())
		{
			mBatches[materialID][mesh->ID] = std::make_shared<RenderBatch>();
			VertexArrayCreationInfo info;		
			if (mesh->mBones.size())
			{
				info.Layout =
				{
					{ ShaderDataType::Float3,"aPos",0,true },
					{ ShaderDataType::Float3,"aNormal" ,0,true},
					{ ShaderDataType::Float2,"aUV" ,0,true},
					{ ShaderDataType::Int4,"aBoneID",0,true  },
					{ ShaderDataType::Float4,"aBoneWeights",0,true  },
					{ ShaderDataType::Int4,"aEntityID",1 },
					{ ShaderDataType::Mat4, "instanceMatrix",1 },
				};

				GraphicsDevice::Instance()->CreateVertexArray(mBatches[materialID][mesh->ID]->VAO, info);
				GraphicsDevice::Instance()->VertexArray_BindAttributes(mBatches[materialID][mesh->ID]->VAO, info);

				{
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 0, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, 0);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 1, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, sizeof(glm::vec3));
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 2, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, sizeof(glm::vec3) * 2);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 3, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, (sizeof(glm::vec3) * 2) + sizeof(glm::vec2));
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 4, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, (sizeof(glm::vec3) * 2) + sizeof(glm::vec2) + sizeof(glm::ivec4));

					GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(mBatches[materialID][mesh->ID]->VAO, mesh->EBO_Buffer->RenderID);
				}
				{
					BufferDescription desc;
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(glm::vec4) * 1024;
					desc.usage = BufferUsage::Stream;
					desc.Name = "EntityIDBuffer";
					desc.data = nullptr;
					mBatches[materialID][mesh->ID]->EntityID_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EntityID_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 5, sizeof(glm::vec4), mBatches[materialID][mesh->ID]->EntityID_Buffer->RenderID);

					GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 5, 1);
				}
				{
					BufferDescription desc;
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(glm::mat4) * 1024;
					desc.usage = BufferUsage::Stream;
					desc.Name = "MatrixBuffer";
					desc.data = nullptr;
					mBatches[materialID][mesh->ID]->Matrix_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->Matrix_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 6, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID, 0);

					GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 6, 1);

				}
			}
			else
			{
				info.Layout =
				{
					{ ShaderDataType::Float3,"aPos",0,true },
					{ ShaderDataType::Float3,"aNormal" ,0,true},
					{ ShaderDataType::Float2,"aUV" ,0,true},
					{ ShaderDataType::Int4,"aEntityID",1 },
					{ ShaderDataType::Mat4, "instanceMatrix",1 },
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
					desc.usage = BufferUsage::Stream;
					desc.Name = "EntityIDBuffer";
					desc.data = nullptr;
					mBatches[materialID][mesh->ID]->EntityID_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EntityID_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 3, sizeof(glm::vec4), mBatches[materialID][mesh->ID]->EntityID_Buffer->RenderID);

					GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 3, 1);
				}
				{
					BufferDescription desc;
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(glm::mat4) * 1024;
					desc.usage = BufferUsage::Stream;
					desc.Name = "MatrixBuffer";
					desc.data = nullptr;
					mBatches[materialID][mesh->ID]->Matrix_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->Matrix_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 4, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID, 0);

					GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 4, 1);

				}
			}
		}

		glm::vec4 color(0);
		uint32_t id = entity;
		uint8_t r = (id >> 24) & 0xFF; // Red component
		uint8_t g = (id >> 16) & 0xFF; // Green component
		uint8_t b = (id >> 8) & 0xFF;  // Blue component
		uint8_t a = id & 0xFF;         // Alpha component
		color.r = (float)r / 255.0f;
		color.g = (float)g / 255.0f;
		color.b = (float)b / 255.0f;
		color.a = (float)a / 255.0f;

		mBatches[materialID][mesh->ID]->EntityIDs.push_back(color);
		mBatches[materialID][mesh->ID]->Transforms.push_back(transform);


		const auto& material = AssetManager::GetMaterialByID(materialID);

		auto drawcall = std::make_shared<MeshDrawCall>();
		drawcall->EntityID = entity;
		drawcall->basevertex = submesh.BaseVertex;
		drawcall->IndexCount = submesh.IndexCount;
		drawcall->SizeOfindices = (void*)(sizeof(unsigned int) * submesh.BaseIndex);


		if (material->IsTranslucent)
			mBatches[materialID][mesh->ID]->TransparentDrawCalls.push_back(drawcall);
		else
			mBatches[materialID][mesh->ID]->OpaqueDrawCalls.push_back(drawcall);

		if (material->CastsShadows)
			mBatches[materialID][mesh->ID]->ShadowDrawCalls.push_back(drawcall);
	}	
}

void Fracture::RenderContext::AddDrawCall(MeshComponent* meshcomp, glm::mat4 transform, UUID entity)
{
	OPTICK_EVENT();
	if (!AssetManager::Instance()->IsMeshLoaded(meshcomp->Mesh))
		return;

	glm::vec4 color(0);
	uint32_t id = entity;
	uint8_t r = (id >> 24) & 0xFF; // Red component
	uint8_t g = (id >> 16) & 0xFF; // Green component
	uint8_t b = (id >> 8) & 0xFF;  // Blue component
	uint8_t a = id & 0xFF;         // Alpha component
	color.r = (float)r / 255.0f;
	color.g = (float)g / 255.0f;
	color.b = (float)b / 255.0f;
	color.a = (float)a / 255.0f;


	const auto& mesh = AssetManager::GetStaticByIDMesh(meshcomp->Mesh);

	for (const auto& submesh : mesh->SubMeshes)
	{
		auto materialID = meshcomp->Materials[submesh.MaterialIndex];
		const auto& material = AssetManager::GetMaterialByID(materialID);

		auto drawcall = std::make_shared<MeshDrawCall>();

		drawcall->EntityID = entity;
		drawcall->IDColor = color;
		drawcall->model = transform;
		drawcall->MaterialID = materialID;
		drawcall->MeshHandle = mesh->VAO;
		drawcall->basevertex = submesh.BaseVertex;
		drawcall->IndexCount = submesh.IndexCount;
		drawcall->SizeOfindices = (void*)(sizeof(unsigned int) * submesh.BaseIndex);


		if (material->IsTranslucent)
			TransparentDrawCalls.push_back(drawcall);
		else
			OpaqueDrawCalls.push_back(drawcall);

		if (material->CastsShadows)
			ShadowDrawCalls.push_back(drawcall);
	}
}

void Fracture::RenderContext::DrawOutlines(UUID entity)
{
	if (SceneManager::HasComponent<MeshComponent>(entity))
	{
		const auto& meshcomp = SceneManager::GetComponent<MeshComponent>(entity);
		const auto& mesh = AssetManager::GetStaticByIDMesh(meshcomp->Mesh);
		const auto& transform = SceneManager::GetComponent<TransformComponent>(entity)->WorldTransform;

		for (const auto& submesh : mesh->SubMeshes)
		{
			auto materialID = meshcomp->Materials[submesh.MaterialIndex];
			const auto& material = AssetManager::GetMaterialByID(materialID);

			auto drawcall = std::make_shared<MeshDrawCall>();

			drawcall->EntityID = entity;
			drawcall->model = transform;
			drawcall->MaterialID = materialID;
			drawcall->MeshHandle = mesh->VAO;
			drawcall->basevertex = submesh.BaseVertex;
			drawcall->IndexCount = submesh.IndexCount;
			drawcall->SizeOfindices = (void*)(sizeof(unsigned int) * submesh.BaseIndex);

			OutlineDrawCalls.push_back(drawcall);
		}
	}
}

void Fracture::RenderContext::DrawPrefabOutlines(UUID entity)
{
	const auto& prefabInstancecomponents = SceneManager::GetAllComponents<PrefabInstanceComponent>();
	for (const auto& prefab : prefabInstancecomponents)
	{
		if (prefab->Parent_PrefabID == entity)
		{
			
			const auto& mesh = AssetManager::GetStaticByIDMesh(prefab->Mesh);
			const auto& transform = SceneManager::GetComponent<TransformComponent>(prefab->EntityID)->WorldTransform;

			for (const auto& submesh : mesh->SubMeshes)
			{
				auto materialID = prefab->Materials[submesh.MaterialIndex];
				const auto& material = AssetManager::GetMaterialByID(materialID);

				auto drawcall = std::make_shared<MeshDrawCall>();

				drawcall->EntityID = entity;
				drawcall->model = transform;
				drawcall->MaterialID = materialID;
				drawcall->MeshHandle = mesh->VAO;
				drawcall->basevertex = submesh.BaseVertex;
				drawcall->IndexCount = submesh.IndexCount;
				drawcall->SizeOfindices = (void*)(sizeof(unsigned int) * submesh.BaseIndex);

				OutlineDrawCalls.push_back(drawcall);
			}
			
		}
	}
	
}

void Fracture::RenderContext::AddToBatch(PrefabInstanceComponent* meshcomponent, glm::mat4 transform, UUID entity)
{
	if (!AssetManager::Instance()->IsMeshLoaded(meshcomponent->Mesh))
		return;

	const auto& mesh = AssetManager::GetStaticByIDMesh(meshcomponent->Mesh);

	for (const auto& submesh : mesh->SubMeshes)
	{
		auto materialID = meshcomponent->Materials[submesh.MaterialIndex];
		if (mBatches.find(materialID) == mBatches.end() || mBatches[materialID].find(mesh->ID) == mBatches[materialID].end())
		{
			mBatches[materialID][mesh->ID] = std::make_shared<RenderBatch>();
			VertexArrayCreationInfo info;
			
			bool IsSkinned = mesh->mBones.size();		
			if (IsSkinned)
			{
				info.Layout =
				{
					{ ShaderDataType::Float3,"aPos",0,true },
					{ ShaderDataType::Float3,"aNormal" ,0,true},
					{ ShaderDataType::Float2,"aUV" ,0,true},
					{ ShaderDataType::Int4,"aBoneID",0 },
					{ ShaderDataType::Float4,"aBoneWeights",0 },
					{ ShaderDataType::Int4,"aEntityID",1 },
					{ ShaderDataType::Mat4, "instanceMatrix",1 },
				};

				GraphicsDevice::Instance()->CreateVertexArray(mBatches[materialID][mesh->ID]->VAO, info);
				GraphicsDevice::Instance()->VertexArray_BindAttributes(mBatches[materialID][mesh->ID]->VAO, info);

				{
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 0, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID, 0);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 1, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID, sizeof(glm::vec3));
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 2, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID, sizeof(glm::vec3) * 2);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 3, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID, (sizeof(glm::vec3) * 2) + sizeof(glm::vec2));
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 4, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID, (sizeof(glm::vec3) * 2) + sizeof(glm::vec2) + sizeof(glm::ivec4));
					GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(mBatches[materialID][mesh->ID]->VAO, mesh->EBO_Buffer->RenderID);
				}
				{
					BufferDescription desc;
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(glm::vec4) * 1024;
					desc.usage = BufferUsage::Stream;
					desc.Name = "EntityIDBuffer";
					desc.data = nullptr;
					mBatches[materialID][mesh->ID]->EntityID_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EntityID_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 5, sizeof(glm::vec4), mBatches[materialID][mesh->ID]->EntityID_Buffer->RenderID);

					GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 5, 1);
				}
				{
					BufferDescription desc;
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(glm::mat4) * 1024;
					desc.usage = BufferUsage::Stream;
					desc.Name = "MatrixBuffer";
					desc.data = nullptr;
					mBatches[materialID][mesh->ID]->Matrix_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->Matrix_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 6, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID, 0);

					GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 6, 1);

				}
			}
			else
			{
				info.Layout =
				{
					{ ShaderDataType::Float3,"aPos",0,true },
					{ ShaderDataType::Float3,"aNormal" ,0,true},
					{ ShaderDataType::Float2,"aUV" ,0,true},
					{ ShaderDataType::Int4,"aEntityID",1 },
					{ ShaderDataType::Mat4, "instanceMatrix",1 },
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
					desc.usage = BufferUsage::Stream;
					desc.Name = "EntityIDBuffer";
					desc.data = nullptr;
					mBatches[materialID][mesh->ID]->EntityID_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->EntityID_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 3, sizeof(glm::vec4), mBatches[materialID][mesh->ID]->EntityID_Buffer->RenderID);

					GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 3, 1);
				}
				{
					BufferDescription desc;
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(glm::mat4) * 1024;
					desc.usage = BufferUsage::Stream;
					desc.Name = "MatrixBuffer";
					desc.data = nullptr;
					mBatches[materialID][mesh->ID]->Matrix_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mBatches[materialID][mesh->ID]->Matrix_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mBatches[materialID][mesh->ID]->VAO, 4, sizeof(glm::mat4), mBatches[materialID][mesh->ID]->Matrix_Buffer->RenderID, 0);

					GraphicsDevice::Instance()->VertexArray_SetDivisor(mBatches[materialID][mesh->ID]->VAO, 4, 1);

				}
			}

			
		}



		glm::vec4 color(0);
		uint32_t id = entity;
		uint8_t r = (id >> 24) & 0xFF; // Red component
		uint8_t g = (id >> 16) & 0xFF; // Green component
		uint8_t b = (id >> 8) & 0xFF;  // Blue component
		uint8_t a = id & 0xFF;         // Alpha component
		color.r = (float)r / 255.0f;
		color.g = (float)g / 255.0f;
		color.b = (float)b / 255.0f;
		color.a = (float)a / 255.0f;

		mBatches[materialID][mesh->ID]->EntityIDs.push_back(color);
		mBatches[materialID][mesh->ID]->Transforms.push_back(transform);


		const auto& material = AssetManager::GetMaterialByID(materialID);

		auto drawcall = std::make_shared<MeshDrawCall>();
		drawcall->basevertex = submesh.BaseVertex;
		drawcall->IndexCount = submesh.IndexCount;
		drawcall->SizeOfindices = (void*)(sizeof(unsigned int) * submesh.BaseIndex);


		if (material->IsTranslucent)
			mBatches[materialID][mesh->ID]->TransparentDrawCalls.push_back(drawcall);
		else
			mBatches[materialID][mesh->ID]->OpaqueDrawCalls.push_back(drawcall);

		if (material->CastsShadows)
			mBatches[materialID][mesh->ID]->ShadowDrawCalls.push_back(drawcall);
	}
}

void Fracture::RenderContext::ResetBatches()
{
	for (auto bactcheslist : mBatches)
	{
		for (auto batch : bactcheslist.second)
		{
			batch.second->clear();
		}
	}
}

void Fracture::RenderContext::WriteToStencilBuffer(Fracture::UUID entity)
{
	mStentilTestPass[entity] = true;
}

bool Fracture::RenderContext::IsWrittenOnStencil(Fracture::UUID entity)
{
	return mStentilTestPass[entity];
}

