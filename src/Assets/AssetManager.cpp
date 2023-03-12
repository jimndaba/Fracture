#include "FracturePCH.h"
#include "AssetManager.h"
#include "MeshLoader.h"

#include "Rendering/Mesh.h"
#include "Serialisation/Serialiser.h"
#include "Rendering/GraphicsDevice.h"
#include "Rendering/Shader.h"

std::map<Fracture::UUID, Fracture::MeshRegistry> Fracture::AssetManager::mMeshRegister;
std::map<std::string, Fracture::UUID> Fracture::AssetManager::mMeshIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::StaticMesh>> Fracture::AssetManager::mMeshes;

std::vector<Fracture::UUID> Fracture::AssetManager::mLoadedMeshes;
std::unordered_map<Fracture::UUID, std::future<std::shared_ptr<Fracture::StaticMesh>>> Fracture::AssetManager::mMeshFutures;
std::queue<Fracture::MeshRegistry> Fracture::AssetManager::mMeshesToLoad;

std::map<Fracture::UUID, Fracture::ShaderRegistry>  Fracture::AssetManager::mShaderRegister;
std::map<std::string, Fracture::UUID>  Fracture::AssetManager::mShaderIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::Shader>>  Fracture::AssetManager::mShaders;

std::unique_ptr<Fracture::AssetManager> Fracture::AssetManager::mInstance;

Fracture::AssetManager::AssetManager()
{
}

void Fracture::AssetManager::OnInit(const std::string& assetfilepath)
{
	ISerialiser reg_serialiser = ISerialiser(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
	reg_serialiser.Open(assetfilepath);//"Content/AssetRegister.json"
	
	if (reg_serialiser.BeginStruct("AssetRegistry"))
	{
		if (reg_serialiser.BeginCollection("Mesh Registry"))
		{
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{
				MeshRegistry reg;
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Path = reg_serialiser.STRING("Path");
				reg.meshType = (MeshRegistry::MeshType)reg_serialiser.INT("Type");
				RegisterMesh(reg);
				reg_serialiser.NextInCollection();	
				
			}
			reg_serialiser.EndCollection();
			FRACTURE_TRACE("Loading Mesh Assets");
		}

		if (reg_serialiser.BeginCollection("Shader Registry"))
		{
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{
				ShaderRegistry reg;	
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Vertex_Path = reg_serialiser.STRING("Vertex_Path");
				reg.Fragment_Path = reg_serialiser.STRING("Frag_Path");
				reg.Geometry_Path = reg_serialiser.STRING("Geo_Path");
				reg.Compute_Path = reg_serialiser.STRING("Comp_Path");
				reg.TessalationEval_Path = reg_serialiser.STRING("TesEval_Path");
				reg.TessalationControl_Path = reg_serialiser.STRING("TesControl_Path");
				RegisterShader(reg);
				reg_serialiser.NextInCollection();
			}
			reg_serialiser.EndCollection();
			FRACTURE_TRACE("Loading Shader Assets");
		}

		reg_serialiser.EndStruct();
	}

	FRACTURE_TRACE("Regiseted {} Meshes", mMeshRegister.size());
	FRACTURE_TRACE("Regiseted {} Shaders", mShaderRegister.size());
}

void Fracture::AssetManager::OnSave(const std::string& path)
{
	FRACTURE_INFO("Asset Register Update: {}", path);
	ISerialiser reg_serialiser = ISerialiser(Fracture::ISerialiser::IOMode::Save, Fracture::ISerialiser::SerialiseFormat::Json);

	reg_serialiser.BeginStruct("AssetRegistry");

	reg_serialiser.BeginCollection("Mesh Registry");
	for (const auto& reg : mMeshRegister)
	{
		reg_serialiser.Property("Mesh", reg.second);
	}
	reg_serialiser.EndCollection();

	reg_serialiser.BeginCollection("Shader Registry");
	for (const auto& reg : mShaderRegister)
	{
		reg_serialiser.Property("Shader", reg.second);
	}
	reg_serialiser.EndCollection();


	reg_serialiser.EndStruct();


	reg_serialiser.Save(path);
	FRACTURE_INFO("Asset Register Update: {}", path);
}

void Fracture::AssetManager::OnLoad()
{
	while (!mMeshesToLoad.empty())
	{
		auto reg = mMeshesToLoad.front();
		mMeshFutures[reg.ID]  = std::async(std::launch::async, [reg]() { return MeshLoader::LoadStaticMesh(reg.Path); });		
		mMeshesToLoad.pop();
	}

	for (auto& mf : mMeshFutures)
	{
		if (mMeshFutures.empty())
			return;

		if (mf.second._Is_ready())
		{
			auto mesh = mf.second.get();
			{
				VertexArrayCreationInfo info;
				info.Layout =
				{
					{ ShaderDataType::Float3,"aPos" },
					{ ShaderDataType::Float3,"aNormal" },
					{ ShaderDataType::Float2,"aUV" },
					{ ShaderDataType::Mat4,"instanceMatrix;" }
				};
				
				GraphicsDevice::Instance()->CreateVertexArray(mesh->VAO, info);
				
				{
					BufferDescription desc;
					desc.data = mesh->mPositions.data();
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(mesh->mPositions[0]) * mesh->mPositions.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "aPos";
					auto buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 0, sizeof(mesh->mPositions[0]), buffer->RenderID);
				}
				{
					BufferDescription desc;
					desc.data = mesh->mNormals.data();
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(mesh->mNormals[0]) * mesh->mNormals.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "aNormal";
					auto buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 1, sizeof(mesh->mNormals[0]), buffer->RenderID);
				}
				{
					BufferDescription desc;
					desc.data = mesh->mUVs.data();
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(mesh->mUVs[0]) * mesh->mUVs.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "aUV";
					auto buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 2, sizeof(mesh->mUVs[0]), buffer->RenderID);
				}
				{
					BufferDescription desc;
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(glm::mat4) * 8000;
					desc.usage = BufferUsage::Static;
					desc.Name = "MatrixBuffer";
					desc.data = nullptr;
					mesh->Matrix_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mesh->Matrix_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 3, sizeof(glm::mat4), mesh->Matrix_Buffer->RenderID);
				}
				{
					BufferDescription desc;
					desc.data = mesh->Indices.data();
					desc.bufferType = BufferType::ElementArrayBuffer;
					desc.size = sizeof(mesh->Indices[0]) * mesh->Indices.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "IndexBuffer";
					auto buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(mesh->VAO, buffer->RenderID);
				}
					

				GraphicsDevice::Instance()->VertexArray_BindAttributes(mesh->VAO, info);
			}

			mesh->ID = mf.first;
			mesh->mPositions.clear();
			mesh->mNormals.clear();
			//mesh->Indices.clear();
			mesh->mUVs.clear();
			mesh->SubMeshes.clear();


			mLoadedMeshes.push_back(mesh->ID);
			mMeshes[mf.first] = mesh;		

			mMeshFutures.erase(mf.first);
		}
	}
}

void Fracture::AssetManager::RegisterMesh(const MeshRegistry& reg)
{
	if (!HasMeshPath(reg.Path))
	{
		mMeshRegister[reg.ID] = reg;
		mMeshIDLookUp[reg.Name] = reg.ID;
	}
}

void Fracture::AssetManager::LoadMesh(const std::string& Name)
{
	auto mesh_ID = mMeshIDLookUp[Name];
	auto it = mMeshRegister.find(mesh_ID);
	if (it != mMeshRegister.end())
	{
		MeshLoader loader{};
		auto mesh = loader.LoadStaticMesh(it->second.Path);
		mMeshes[mesh_ID] = mesh;
	}
}

Fracture::Shader* Fracture::AssetManager::GetShader(const std::string& Name)
{
	{
		auto it = mShaderIDLookUp.find(Name);
		if (it != mShaderIDLookUp.end())
		{
			auto check_shader = mShaders.find(it->second);
			if (check_shader == mShaders.end())
			{
				ShaderDescription desc;
				desc.VertexPath = mShaderRegister[it->second].Vertex_Path;
				desc.FragmentPath = mShaderRegister[it->second].Fragment_Path;
				desc.GeometryPath = mShaderRegister[it->second].Geometry_Path;
				desc.ComputePath = mShaderRegister[it->second].Compute_Path;
				desc.TesselationControlPath = mShaderRegister[it->second].TessalationControl_Path;
				desc.TesselationEvaluationPath = mShaderRegister[it->second].TessalationEval_Path;
				desc.Name = Name;
				mShaders[it->second] = GraphicsDevice::Instance()->CreateShader(desc);
				mShaders[it->second]->IsLoaded = true;
				mShaders[it->second]->ID = it->second;
			}
			return mShaders[it->second].get();
		}
	}
	FRACTURE_ERROR("Could not find Shader: {}", Name);
	return nullptr;
}

Fracture::Shader* Fracture::AssetManager::GetShaderByID(const Fracture::UUID& id)
{
	{
		auto it = mShaders.find(id);	
		if (it != mShaders.end())
		{
			if (!mShaders[id]->IsLoaded)
			{
				ShaderDescription desc;
				desc.VertexPath = mShaderRegister[id].Vertex_Path;
				desc.FragmentPath = mShaderRegister[id].Fragment_Path;
				desc.GeometryPath = mShaderRegister[id].Geometry_Path;
				desc.ComputePath = mShaderRegister[id].Compute_Path;
				desc.TesselationControlPath = mShaderRegister[id].TessalationControl_Path;
				desc.TesselationEvaluationPath = mShaderRegister[id].TessalationEval_Path;
				desc.Name = mShaderRegister[id].Name;
				mShaders[id] = GraphicsDevice::Instance()->CreateShader(desc);
				mShaders[id]->IsLoaded = true;
				mShaders[id]->ID = id;
			}
			return mShaders[id].get();
		}
	}
	FRACTURE_ERROR("Could not find Shader: {}", id);
	return nullptr;
}

void Fracture::AssetManager::AsyncLoadMesh(const std::string& Name)
{
	auto it = mMeshIDLookUp.find(Name);
	if (it != mMeshIDLookUp.end())
	{
		mMeshesToLoad.push(mMeshRegister[it->second]);
	}
}

void Fracture::AssetManager::AsyncLoadMeshByID(const UUID& id)
{
	auto it = mMeshRegister.find(id);
	if (it != mMeshRegister.end())
	{
		mMeshesToLoad.push(mMeshRegister[it->first]);
	}
}

bool Fracture::AssetManager::IsMeshLoaded(const std::string& Name)
{
	return std::find(mLoadedMeshes.begin(), mLoadedMeshes.end(), mMeshIDLookUp[Name]) != mLoadedMeshes.end();
}

bool Fracture::AssetManager::IsMeshLoaded(const UUID& id)
{
	return std::find(mLoadedMeshes.begin(), mLoadedMeshes.end(), id) != mLoadedMeshes.end();
}

Fracture::UUID Fracture::AssetManager::GetMeshID(const std::string& Name)
{
	auto it = mMeshIDLookUp.find(Name);
	if (it != mMeshIDLookUp.end())
	{
		return it->second;
	}
}

Fracture::StaticMesh* Fracture::AssetManager::GetStaticMesh(const std::string& Name)
{
	{
		auto it = mMeshIDLookUp.find(Name);
		if (it != mMeshIDLookUp.end())
		{
			return mMeshes[it->second].get();
		}
	}
	return nullptr;
}

Fracture::StaticMesh* Fracture::AssetManager::GetStaticByIDMesh(const UUID& id)
{
	{
		auto it = mMeshes.find(id);
		if (it != mMeshes.end())
		{
			return mMeshes[id].get();
		}
	}
	return nullptr;
}

bool Fracture::AssetManager::HasMeshPath(const std::string& path)
{
	for (const auto& reg : mMeshRegister)
	{
		if (reg.second.Path == path)
			return true;
	}
	return false;
}

void Fracture::AssetManager::RegisterShader(const ShaderRegistry& reg)
{
	mShaderRegister[reg.ID] = reg;
	mShaderIDLookUp[reg.Name] = reg.ID;
}

Fracture::AssetManager* Fracture::AssetManager::Instance()
{
	if (mInstance)
		return mInstance.get();

	mInstance = std::make_unique<AssetManager>();
	return mInstance.get();
}