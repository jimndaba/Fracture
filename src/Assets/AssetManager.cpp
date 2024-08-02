#include "FracturePCH.h"
#include "AssetManager.h"
#include "MeshLoader.h"
#include "Assets/ImagerLoader.h"

#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Serialisation/Serialiser.h"
#include "MaterialLoader.h"
#include "AnimationClipLoader.h"
#include "Serialisation/MaterialSerialiser.h"
#include "Rendering/GraphicsDevice.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Animation/AnimationClip.h"
#include "World/SceneManager.h"
#include "Scripting/ScriptManager.h"
#include "Serialisation/AnimationGraphSerialiser.h"
#include "Particles/ParticleSystem.h"
#include "Common/Helpers.h"

std::map<Fracture::UUID, Fracture::MeshRegistry> Fracture::AssetManager::mMeshRegister;
std::map<std::string, Fracture::UUID> Fracture::AssetManager::mMeshIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::StaticMesh>> Fracture::AssetManager::mMeshes;

std::vector<Fracture::UUID> Fracture::AssetManager::mLoadedMeshes;
std::unordered_map<Fracture::UUID, std::future<std::shared_ptr<Fracture::StaticMesh>>> Fracture::AssetManager::mMeshFutures;

std::queue<Fracture::MeshRegistry> Fracture::AssetManager::mMeshesToLoad;
std::queue<Fracture::LoadandAttachContext> Fracture::AssetManager::mMeshesToLoadandAttach;
std::unordered_map<Fracture::LoadandAttachContext, std::future<std::shared_ptr<Fracture::StaticMesh>>>  Fracture::AssetManager::mMeshAndAttachFutures;


std::map<Fracture::UUID, Fracture::ShaderRegistry>  Fracture::AssetManager::mShaderRegister;
std::map<std::string, Fracture::UUID>  Fracture::AssetManager::mShaderIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::Shader>>  Fracture::AssetManager::mShaders;

std::map<Fracture::UUID, Fracture::TextureRegistry> Fracture::AssetManager::mTextureRegister;
std::map<std::string, Fracture::UUID> Fracture::AssetManager::mTextureIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::Texture>> Fracture::AssetManager::mTextures;

std::vector<Fracture::UUID> Fracture::AssetManager::mLoadedTextures;
std::unordered_map<Fracture::UUID, std::future<std::shared_ptr<Fracture::Texture>>> Fracture::AssetManager::mTextureFutures;
std::queue<Fracture::TextureRegistry> Fracture::AssetManager::mTexturesToLoad;

std::vector<Fracture::UUID> Fracture::AssetManager::mLoadedMaterials;
std::unordered_map<Fracture::UUID, std::future<std::shared_ptr<Fracture::Material>>> Fracture::AssetManager::mMaterialFutures;
std::queue<Fracture::MaterialRegistry> Fracture::AssetManager::mMaterialToLoad;

std::map<Fracture::UUID, Fracture::MaterialRegistry> Fracture::AssetManager::mMaterialRegister;
std::map<std::string, Fracture::UUID> Fracture::AssetManager::mMaterialIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::Material>> Fracture::AssetManager::mMaterials;

std::vector<Fracture::UUID> Fracture::AssetManager::mLoadedAnimations;
std::unordered_map<Fracture::UUID, std::future<std::shared_ptr<Fracture::AnimationClip>>> Fracture::AssetManager::mAnimationFutures;
std::queue<Fracture::AnimationClipRegistry> Fracture::AssetManager::mAnimationsToLoad;

std::map<Fracture::UUID, Fracture::AnimationClipRegistry> Fracture::AssetManager::mAnimationRegister;
std::map<std::string, Fracture::UUID> Fracture::AssetManager::mAnimationIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::AnimationClip>> Fracture::AssetManager::mAnimations;

std::map<Fracture::UUID, Fracture::AnimationGraphRegistry> Fracture::AssetManager::mAnimationGraphRegister;
std::map<std::string, Fracture::UUID> Fracture::AssetManager::mAnimationGraphIDLookUp;

std::map<Fracture::UUID, Fracture::TerrainRegistry> Fracture::AssetManager::mTerrainRegister;
std::map<std::string, Fracture::UUID> Fracture::AssetManager::mTerrainIDLookUp;
std::unordered_map<Fracture::UUID, std::shared_ptr<Fracture::Terrain>> Fracture::AssetManager::mTerrains;

std::vector<Fracture::UUID> Fracture::AssetManager::mLoadedTerrains;
std::unordered_map<Fracture::UUID, std::future<std::shared_ptr<Fracture::Terrain>>> Fracture::AssetManager::mTerrainFutures;
std::queue<Fracture::TerrainRegistry> Fracture::AssetManager::mTerrainToLoad;


std::unique_ptr<Fracture::AssetManager> Fracture::AssetManager::mInstance;

bool Fracture::AssetManager::IsRegisterDirty = true;
tf::Taskflow  Fracture::AssetManager::taskflow;

Fracture::AssetManager::AssetManager()
{
}

void Fracture::AssetManager::RegisterCallbacks(Eventbus* bus)
{
	bus->Subscribe(this, &Fracture::AssetManager::OnAsyncLoadMesh);
	bus->Subscribe(this, &Fracture::AssetManager::OnAsyncLoadTexture);
	bus->Subscribe(this, &Fracture::AssetManager::OnAsyncLoadandAttach);
	bus->Subscribe(this, &Fracture::AssetManager::OnLoadTexture);

}

void Fracture::AssetManager::OnInit(const std::string& assetfilepath)
{

	ISerialiser reg_serialiser = ISerialiser(Fracture::ISerialiser::IOMode::Open, Fracture::ISerialiser::SerialiseFormat::Json);
	reg_serialiser.Open(assetfilepath);//"Content/AssetRegister.json"
	
	if (reg_serialiser.BeginStruct("AssetRegistry"))
	{
		if (reg_serialiser.BeginCollection("Mesh Registry"))
		{
			FRACTURE_TRACE("Loading Mesh Assets");
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
			
		}

		if (reg_serialiser.BeginCollection("Texture Registry"))
		{
			FRACTURE_TRACE("Loading Texture Assets");
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{
				TextureRegistry reg;
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Path = reg_serialiser.STRING("Path");	
				reg.format = (TextureFormat)reg_serialiser.INT("Format");
				reg.formatType = (TextureFormatType)reg_serialiser.INT("FormatType");
				reg.GenMinMaps = reg_serialiser.BOOL("GenMipmaps");				
				reg.internalFormat = (InternalFormat)reg_serialiser.INT("InternalFormat");
				reg.magFilter =(TextureMagFilter) reg_serialiser.INT("MagFilter");
				reg.minFilter =(TextureMinFilter) reg_serialiser.INT("MinFilter");
				reg.Wrap =(TextureWrap)reg_serialiser.INT("Wrap");
				RegisterTexture(reg);
				reg_serialiser.NextInCollection();

			}
			reg_serialiser.EndCollection();

		}

		if (reg_serialiser.BeginCollection("Shader Registry"))
		{
			FRACTURE_TRACE("Loading Shader Assets");
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
		}

		if (reg_serialiser.BeginCollection("Scene Registry"))
		{
			FRACTURE_TRACE("Loading Scene Assets");
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{
				SceneRegistry reg;
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Path = reg_serialiser.STRING("Path");				
				SceneManager::RegisterScene(reg);
				reg_serialiser.NextInCollection();
			}
			reg_serialiser.EndCollection();			
		}

		if (reg_serialiser.BeginCollection("Material Registry"))
		{
			FRACTURE_TRACE("Loading Material Assets");
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{
				MaterialRegistry reg;
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Path = reg_serialiser.STRING("Path");
				RegisterMaterial(reg);
				reg_serialiser.NextInCollection();
			}
			reg_serialiser.EndCollection();
		}

		if (reg_serialiser.BeginCollection("Script Registry"))
		{
			FRACTURE_TRACE("Loading Script Assets");
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{
				
				LuaScriptRegistry reg;
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Path = reg_serialiser.STRING("Path");
				reg.MetaPath = reg_serialiser.STRING("MetaPath");
				ScriptManager::RegisterScript(reg);
				reg_serialiser.NextInCollection();
			}
			reg_serialiser.EndCollection();
		}

		if (reg_serialiser.BeginCollection("Animation Registry"))
		{
			FRACTURE_TRACE("Loading Animation Assets");
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{

				AnimationClipRegistry reg;
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Path = reg_serialiser.STRING("Path");
				RegisterAnimation(reg);
				reg_serialiser.NextInCollection();
			}
			reg_serialiser.EndCollection();
		}

		if (reg_serialiser.BeginCollection("AnimationGraph Registry"))
		{
			FRACTURE_TRACE("Loading Animation Graph Assets");
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{

				AnimationGraphRegistry reg;
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Path = reg_serialiser.STRING("Path");
				RegisterAnimationGraph(reg);
				reg_serialiser.NextInCollection();
			}
			reg_serialiser.EndCollection();
		}

		if (reg_serialiser.BeginCollection("ParticleFx Registry"))
		{
			FRACTURE_TRACE("Loading ParticleFX Graph Assets");
			while (reg_serialiser.CurrentCollectionIndex() < reg_serialiser.GetCollectionSize())
			{

				ParticleFxRegistry reg;
				reg.ID = reg_serialiser.ID("ID");
				reg.Name = reg_serialiser.STRING("Name");
				reg.Path = reg_serialiser.STRING("Path");
				ParticleSystem::mParticleFxRegister[reg.ID] = reg;
				reg_serialiser.NextInCollection();
			}
			reg_serialiser.EndCollection();
		}


		reg_serialiser.EndStruct();
	}

	FRACTURE_TRACE("Regiseted {} Meshes", mMeshRegister.size());
	FRACTURE_TRACE("Regiseted {} Shaders", mShaderRegister.size());
	FRACTURE_TRACE("Regiseted {} Materials", mMaterialRegister.size());
	FRACTURE_TRACE("Regiseted {} Textures", mTextureRegister.size());
	FRACTURE_TRACE("Regiseted {} Scripts", ScriptManager::mScriptRegister.size());
	FRACTURE_TRACE("Regiseted {} Animations", mAnimationRegister.size());
	FRACTURE_TRACE("Regiseted {} Animation Graphs", mAnimationGraphRegister.size());
	FRACTURE_TRACE("Regiseted {} ParticleFxs", ParticleSystem::mParticleFxRegister.size());
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

	reg_serialiser.BeginCollection("Scene Registry");
	for (const auto& reg : SceneManager::mSceneRegister)
	{
		reg_serialiser.Property("Scene", reg.second);
	}
	reg_serialiser.EndCollection();

	reg_serialiser.BeginCollection("Texture Registry");
	for (const auto& reg : mTextureRegister)
	{
		reg_serialiser.Property("Texture", reg.second);
	}
	reg_serialiser.EndCollection();

	reg_serialiser.BeginCollection("Script Registry");
	for (const auto& reg : ScriptManager::mScriptRegister)
	{
		reg_serialiser.Property("Script", reg.second);
	}
	reg_serialiser.EndCollection();

	reg_serialiser.BeginCollection("Animation Registry");
	for (const auto& reg : mAnimationRegister)
	{
		reg_serialiser.Property("Animation", reg.second);
	}
	reg_serialiser.EndCollection();

	reg_serialiser.BeginCollection("AnimationGraph Registry");
	for (const auto& reg : mAnimationGraphRegister)
	{
		reg_serialiser.Property("AnimationGraph", reg.second);
	}
	reg_serialiser.EndCollection();

	reg_serialiser.BeginCollection("Material Registry");
	for (const auto& reg : mMaterialRegister)
	{
		reg_serialiser.Property("Material", reg.second);
		MaterialSerialiser s = MaterialSerialiser(Fracture::ISerialiser::IOMode::Save, Fracture::ISerialiser::SerialiseFormat::Json);
		if (IsMaterialLoaded(reg.first))
		{
			if (mMaterials[reg.first]->IsDirty)
			{
				s.WriteMaterial(mMaterials[reg.first].get());
				s.Save(reg.second.Path);
			}
		}		
	}
	reg_serialiser.EndCollection();

	reg_serialiser.BeginCollection("ParticleFx Registry");
	for (const auto& reg : ParticleSystem::mParticleFxRegister)
	{
		reg_serialiser.Property("ParticleFx", reg.second);
	}
	reg_serialiser.EndCollection();

	reg_serialiser.EndStruct();

	reg_serialiser.Save(path);
	IsRegisterDirty = false;
	FRACTURE_INFO("Asset Register Update: {}", path);
}

void Fracture::AssetManager::OnLoad()
{
	while (!mMeshesToLoad.empty())
	{
		auto reg = mMeshesToLoad.front();
		mMeshFutures[reg.ID]  = std::async(std::launch::async, [reg]() { return std::move(MeshLoader::LoadStaticMesh(reg.Path)); });				
		mMeshesToLoad.pop();
	}

	for (auto& mf : mMeshFutures)
	{		
		if (mf.second.valid())
		{
			auto mesh = mf.second.get();
			{
				bool IsSkinned = mesh->mBones.size();
				VertexArrayCreationInfo info;
				if (IsSkinned)
				{
					info.Layout =
					{
						{ ShaderDataType::Float3,"aPos",0,true },
						{ ShaderDataType::Float3,"aNormal" ,0,true},
						{ ShaderDataType::Float2,"aUV" ,0,true},
						{ ShaderDataType::Int4,"aBoneIDs" ,0,true},
						{ ShaderDataType::Float4,"aBoneWeights" ,0,true}
					};
				}
				else
				{
					info.Layout =
					{
						{ ShaderDataType::Float3,"aPos",0,true },
						{ ShaderDataType::Float3,"aNormal" ,0,true},
						{ ShaderDataType::Float2,"aUV" ,0,true}
					};
				}


				GraphicsDevice::Instance()->CreateVertexArray(mesh->VAO, info);

				if (IsSkinned)
				{
					BufferDescription desc;
					desc.data = mesh->mSkinnedVerticies.data();
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(mesh->mSkinnedVerticies[0]) * mesh->mSkinnedVerticies.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "Verticies";
					mesh->VBO_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mesh->VBO_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 0, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID);
				}
				else
				{
					BufferDescription desc;
					desc.data = mesh->mVerticies.data();
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(mesh->mVerticies[0]) * mesh->mVerticies.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "Verticies";
					mesh->VBO_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mesh->VBO_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 0, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID);
				}

				{
					BufferDescription desc;
					desc.data = mesh->Indices.data();
					desc.bufferType = BufferType::ElementArrayBuffer;
					desc.size = sizeof(mesh->Indices[0]) * mesh->Indices.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "IndexBuffer";
					mesh->EBO_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mesh->EBO_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(mesh->VAO, mesh->EBO_Buffer->RenderID);
				}
				GraphicsDevice::Instance()->VertexArray_BindAttributes(mesh->VAO, info);

			}

			mesh->ID = mf.first;
			mLoadedMeshes.push_back(mesh->ID);
			mMeshes[mf.first] = mesh;
			mMeshFutures.erase(mf.first);
		}
	}

	while (!mMeshesToLoadandAttach.empty())
	{
		auto cntxt = mMeshesToLoadandAttach.front();
		auto reg = mMeshRegister[cntxt.Mesh];
		mMeshAndAttachFutures[cntxt] = std::async(std::launch::async, [reg]() { return MeshLoader::LoadStaticMesh(reg.Path); });
		mMeshesToLoadandAttach.pop();
	}

	for (auto& mf : mMeshAndAttachFutures)
	{
		if (mf.second.valid())
		{
			auto mesh = mf.second.get();
			{				
				bool IsSkinned = mesh->mBones.size();
				VertexArrayCreationInfo info;
				if (IsSkinned)
				{
					info.Layout =
					{
						{ ShaderDataType::Float3,"aPos",0,true },
						{ ShaderDataType::Float3,"aNormal" ,0,true},
						{ ShaderDataType::Float2,"aUV" ,0,true},
						{ ShaderDataType::Int4,"aBoneID",0 ,true},
						{ ShaderDataType::Float4,"aBoneWeights",0,true }
					};
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
				}
				
				GraphicsDevice::Instance()->CreateVertexArray(mesh->VAO, info);
				if (IsSkinned)
				{
					BufferDescription desc;
					desc.data = mesh->mSkinnedVerticies.data();
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(mesh->mSkinnedVerticies[0]) * mesh->mSkinnedVerticies.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "Verticies";
					mesh->VBO_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mesh->VBO_Buffer.get(), desc);

					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 0, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 1, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, sizeof(glm::vec3));
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 2, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, sizeof(glm::vec3) * 2);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 3, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, (sizeof(glm::vec3) * 2) + sizeof(glm::vec2));
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 4, sizeof(mesh->mSkinnedVerticies[0]), mesh->VBO_Buffer->RenderID, (sizeof(glm::vec3) * 2) + sizeof(glm::vec2) + sizeof(glm::ivec4));

				}
				else
				{
					BufferDescription desc;
					desc.data = mesh->mVerticies.data();
					desc.bufferType = BufferType::ArrayBuffer;
					desc.size = sizeof(mesh->mVerticies[0]) * mesh->mVerticies.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "Verticies";
					mesh->VBO_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mesh->VBO_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(mesh->VAO, 0, sizeof(mesh->mVerticies[0]), mesh->VBO_Buffer->RenderID);
				}
				{
					BufferDescription desc;
					desc.data = mesh->Indices.data();
					desc.bufferType = BufferType::ElementArrayBuffer;
					desc.size = sizeof(mesh->Indices[0]) * mesh->Indices.size();
					desc.usage = BufferUsage::Static;
					desc.Name = "IndexBuffer";
					mesh->EBO_Buffer = std::make_shared<Buffer>();
					GraphicsDevice::Instance()->CreateBuffer(mesh->EBO_Buffer.get(), desc);
					GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(mesh->VAO, mesh->EBO_Buffer->RenderID);
				}
				GraphicsDevice::Instance()->VertexArray_BindAttributes(mesh->VAO, info);				
			}

			mesh->ID = mf.first.Mesh;
			mLoadedMeshes.push_back(mesh->ID);
			mMeshes[mf.first.Mesh] = mesh;
			mMeshAndAttachFutures.erase(mf.first);
			if (mesh->mBones.size())
				SceneManager::AddComponent<MeshComponent>(mf.first.Entity, mf.first.Mesh, mMeshes[mf.first.Mesh]->mMaterials, true);
			else
				SceneManager::AddComponent<MeshComponent>(mf.first.Entity, mf.first.Mesh, mMeshes[mf.first.Mesh]->mMaterials);

			
		}
	}

	while (!mTexturesToLoad.empty())
	{
		auto reg = mTexturesToLoad.front();
		mTextureFutures[reg.ID] = std::async(std::launch::async, [reg]() { return ImageLoader::LoadTexture(reg.Path); });
		mTexturesToLoad.pop();
	}

	for (auto& mf : mTextureFutures)
	{
		if (mTextureFutures.empty())
			return;

		if (mf.second._Is_ready())
		{
			auto texture = mf.second.get();
			{
				texture->Description.ID = mf.first;
				texture->Description.format = mTextureRegister[mf.first].format;
				texture->Description.internalFormat = mTextureRegister[mf.first].internalFormat;
				texture->Description.Wrap = mTextureRegister[mf.first].Wrap;
				texture->Description.magFilter = mTextureRegister[mf.first].magFilter;
				texture->Description.minFilter = mTextureRegister[mf.first].minFilter;
				texture->Description.GenMinMaps = mTextureRegister[mf.first].GenMinMaps;
				texture->Description.formatType = mTextureRegister[mf.first].formatType;
				mTextures[mf.first] = texture;
				GraphicsDevice::Instance()->CreateTexture(mTextures[mf.first], mTextures[mf.first]->Description);
				mLoadedTextures.push_back(mf.first);
				mTextureFutures.erase(mf.first);
			}
		}
	}

	while (!mMaterialToLoad.empty())
	{		
		if (IsMaterialLoaded(mMaterialToLoad.front().ID))
		{
			mMaterialToLoad.pop();
			continue;
		}

		auto  material = MaterialLoader::LoadMaterial(mMaterialToLoad.front().Path);
		if (material)
		{
			mMaterials[mMaterialToLoad.front().ID] = material;
			mLoadedMaterials.push_back(mMaterialToLoad.front().ID);
		}
		
		else
		{
			FRACTURE_ERROR("Failed to load Material: {}", mMaterialToLoad.front().Path);
		}

		mMaterialToLoad.pop();
	}

	while (!mAnimationsToLoad.empty())
	{
		if (IsAnimationLoaded(mAnimationsToLoad.front().ID))
		{
			mAnimationsToLoad.pop();
			continue;
		}
		
		auto reg = mAnimationsToLoad.front();

		if (mAnimationFutures.find(reg.ID) != mAnimationFutures.end())
		{
			mAnimationsToLoad.pop();
			continue;
		}
			
		mAnimationFutures[reg.ID] = std::async(std::launch::async, [reg]() { return AnimationClipLoader::LoadAnimationClip(reg.Path); });
		FRACTURE_TRACE("Async Loading Animaiton {}", reg.Name);
		mAnimationsToLoad.pop();
	}

	for (auto& mf : mAnimationFutures)
	{
		if (mAnimationFutures.empty())
			return;

		if (mf.second._Is_ready())
		{
			auto animation = mf.second.get();
			{
				FRACTURE_TRACE("Animaiton Loaded{}", animation->Name);
				mAnimations[mf.first] = animation;
				mLoadedAnimations.push_back(mf.first);
				mAnimationFutures.erase(mf.first);
			}
		}
	}
}

void Fracture::AssetManager::RegisterMesh(const MeshRegistry& reg)
{
	if (!HasMeshPath(reg.Path) && reg.IsValid)
	{
		mMeshRegister[reg.ID] = reg;
		mMeshIDLookUp[reg.Name] = reg.ID;
		IsRegisterDirty = true;
		FRACTURE_TRACE("Registering Mesh: {}", reg.Path);
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
		mLoadedMeshes.push_back(mesh_ID);
	}
}

std::shared_ptr<Fracture::Shader> Fracture::AssetManager::GetShader(const std::string& Name)
{
	{
		auto it = mShaderRegister.find(mShaderIDLookUp[Name]);
		if (it != mShaderRegister.end())
		{
			auto findshader = mShaders.find(it->first);
			if (findshader == mShaders.end())
			{
				ShaderDescription desc;
				desc.VertexPath = mShaderRegister[it->first].Vertex_Path;
				desc.FragmentPath = mShaderRegister[it->first].Fragment_Path;
				desc.GeometryPath = mShaderRegister[it->first].Geometry_Path;
				desc.ComputePath = mShaderRegister[it->first].Compute_Path;
				desc.TesselationControlPath = mShaderRegister[it->first].TessalationControl_Path;
				desc.TesselationEvaluationPath = mShaderRegister[it->first].TessalationEval_Path;
				desc.Name = Name;
				mShaders[it->first] = GraphicsDevice::Instance()->CreateShader(desc);
				mShaders[it->first]->IsLoaded = true;
				mShaders[it->first]->ID = it->first;
				FRACTURE_TRACE("Created Shader : {}", Name);
			}
			return mShaders[it->first];
		}
	}
	FRACTURE_ERROR("Could not find Shader: {}", Name);
	return nullptr;
}

std::shared_ptr<Fracture::Shader> Fracture::AssetManager::GetShaderByID(const Fracture::UUID& id)
{
	{
		auto it = mShaderRegister.find(id);
		if (it != mShaderRegister.end())
		{
			if (mShaders[id])
				return mShaders[id];

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
			return mShaders[id];
		}
	}
	FRACTURE_ERROR("Could not find Shader: {}", id);
	return nullptr;
}

void Fracture::AssetManager::RegisterTexture(const TextureRegistry& reg)
{
	mTextureRegister[reg.ID] = reg;
	mTextureIDLookUp[reg.Name] = reg.ID;
	IsRegisterDirty = true;

	FRACTURE_TRACE("Registering Texture:{} - {} ",reg.ID, reg.Path);
}

std::shared_ptr<Fracture::Texture>Fracture::AssetManager::GetTexture(const std::string& Name)
{
	{
		auto it = mTextureIDLookUp.find(Name);
		if (it != mTextureIDLookUp.end())
		{
			auto check_texture = mTextures.find(it->second);
			if (check_texture != mTextures.end())
			{	
				return mTextures[it->second];
			}		
		};
	}
	FRACTURE_ERROR("Could not find Texture: {}", Name);
	return nullptr;
}

std::shared_ptr<Fracture::Texture>Fracture::AssetManager::GetTextureByID(const Fracture::UUID& id)
{
	if (!IsTextureLoaded(id))
	{
		auto it = mTextureRegister.find(id);
		if (it != mTextureRegister.end())
		{
			if (mTextureRegister[id].Path.empty())
			{
				return nullptr;
			}		
			
			mTextures[id] = ImageLoader::LoadTexture(mTextureRegister[id].Path);
			mTextures[id]->Description.ID = id;
			mTextures[id]->Description.format = mTextureRegister[id].format;
			mTextures[id]->Description.internalFormat = mTextureRegister[id].internalFormat;
			mTextures[id]->Description.Wrap = mTextureRegister[id].Wrap;
			mTextures[id]->Description.magFilter = mTextureRegister[id].magFilter;
			mTextures[id]->Description.minFilter = mTextureRegister[id].minFilter;
			mTextures[id]->Description.GenMinMaps = mTextureRegister[id].GenMinMaps;
			mTextures[id]->Description.formatType = mTextureRegister[id].formatType;
			GraphicsDevice::Instance()->CreateTexture(mTextures[id], mTextures[id]->Description);
			mLoadedTextures.push_back(id);
			FRACTURE_TRACE("Loaded Texture: {}", mTextureRegister[id].Path);
		}
		else
		{
			FRACTURE_ERROR("Texture: {} , Is not Registered", id);
		}
	}
	else if(IsTextureLoaded(id))
	{
		return mTextures[id];
	}
	FRACTURE_ERROR("Could not find texture: {}", id);
	return nullptr;
}

void Fracture::AssetManager::RegisterMaterial(const MaterialRegistry& reg)
{
	mMaterialRegister[reg.ID] = reg;
	mMaterialIDLookUp[reg.Name] = reg.ID;
	IsRegisterDirty = true;

	FRACTURE_TRACE("Registering Material : {} ", reg.Path);
}

std::shared_ptr<Fracture::Material> Fracture::AssetManager::GetMaterial(const std::string& Name)
{	
	{
		if (!IsMaterialLoaded(Name))
		{
			auto it = mMaterialRegister.find(mMaterialIDLookUp[Name]);
			if (it != mMaterialRegister.end())
				mMaterials[mMaterialIDLookUp[Name]] = MaterialLoader::LoadMaterial(mMaterialRegister[mMaterialIDLookUp[Name]].Path);
		}


		auto it = mMaterialIDLookUp.find(Name);
		if (it != mMaterialIDLookUp.end())
		{
			auto check_texture = mMaterials.find(it->second);
			if (check_texture != mMaterials.end())
			{
				return mMaterials[it->second];
			}
		};
	}
	FRACTURE_ERROR("Could not find Material: {}", Name);
	return nullptr;
}

std::shared_ptr<Fracture::Material> Fracture::AssetManager::GetMaterialByID(const Fracture::UUID& id)
{
	if (!id)
		return nullptr;

	{
		if (!IsMaterialLoaded(mMaterialRegister[id].ID))
		{
			const auto& path = mMaterialRegister[id].Path;
			mMaterials[id] = MaterialLoader::LoadMaterial(path);
			mLoadedMaterials.push_back(id);		
		}

		auto it = mMaterials.find(id);
		if (it != mMaterials.end())
		{
			return mMaterials[id];
		}		
	}
	FRACTURE_ERROR("Could not find Material: {}", id);
	return nullptr;
}

void Fracture::AssetManager::RegisterAnimation(const AnimationClipRegistry& reg)
{
	mAnimationRegister[reg.ID] = reg;
	mAnimationIDLookUp[reg.Name] = reg.ID;
	IsRegisterDirty = true;
	FRACTURE_TRACE("Registering Animation : {} ", reg.Path);
}

std::shared_ptr <Fracture::AnimationClip > Fracture::AssetManager::GetAnimation(const std::string& Name)
{
	{
		if (!IsAnimationLoaded(Name))
		{
			auto it = mAnimationRegister.find(mAnimationIDLookUp[Name]);
			if (it != mAnimationRegister.end())
				mAnimations[mAnimationIDLookUp[Name]] = AnimationClipLoader::LoadAnimationClip(mAnimationRegister[mAnimationIDLookUp[Name]].Path);
		}


		auto it = mAnimationIDLookUp.find(Name);
		if (it != mAnimationIDLookUp.end())
		{
			auto check_animation = mAnimations.find(it->second);
			if (check_animation != mAnimations.end())
			{
				return mAnimations[it->second];
			}
		};
	}
	FRACTURE_ERROR("Could not find Material: {}", Name);
	return nullptr;
}

std::shared_ptr<Fracture::AnimationClip> Fracture::AssetManager::GetAnimationByID(const Fracture::UUID& id)
{
	if (!id)
		return nullptr;
	{
		auto itr = mAnimationRegister.find(id);
		if (itr == mAnimationRegister.end())
		{
			FRACTURE_ERROR("Animation not registered: {}", id);
			return nullptr;
		}

		if (!IsAnimationLoaded(mAnimationRegister[id].ID))
		{		
			const auto& path = mAnimationRegister[id].Path;
			mAnimations[id] = AnimationClipLoader::LoadAnimationClip(path);
			mLoadedAnimations.push_back(id);
		}

		auto it = mAnimations.find(id);
		if (it != mAnimations.end())
		{
			return mAnimations[id];
		}
		
	}
	FRACTURE_ERROR("Could not find Animation: {}", id);
	return nullptr;
}

void Fracture::AssetManager::RegisterAnimationGraph(const AnimationGraphRegistry& reg)
{
	mAnimationGraphRegister[reg.ID] = reg;
	mAnimationGraphIDLookUp[reg.Name] = reg.ID;
	IsRegisterDirty = true;
	FRACTURE_TRACE("Registering Animation Graph: {} ", reg.Path);
}

std::shared_ptr<Fracture::AnimationGraph> Fracture::AssetManager::GetAnimationGraph(const std::string& Name)
{
	auto it = mAnimationGraphRegister.find(mAnimationGraphIDLookUp[Name]);
	if (it != mAnimationGraphRegister.end())
	{
		auto reader = AnimationGraphSerialiser(Fracture::ISerialiser::IOMode::Open, ISerialiser::SerialiseFormat::Json);
		reader.Open(mAnimationGraphRegister[mAnimationGraphIDLookUp[Name]].Path);
		auto graph = reader.ReadGraph();

		if (graph)
			return graph;
	}
	FRACTURE_ERROR("Could not read Animationgrpah: {}", Name);
	return nullptr;
}

std::shared_ptr<Fracture::AnimationGraph> Fracture::AssetManager::GetAnimationGraphByID(const Fracture::UUID& id)
{
	auto it = mAnimationGraphRegister.find(id);
	if (it != mAnimationGraphRegister.end())
	{
		auto reader = AnimationGraphSerialiser(Fracture::ISerialiser::IOMode::Open, ISerialiser::SerialiseFormat::Json);
		reader.Open(mAnimationGraphRegister[id].Path);
		auto graph = reader.ReadGraph();

		if (graph)
			return graph;
	}
	FRACTURE_ERROR("Could not read Animationgrpah: {}", id);
	return nullptr;
}

void Fracture::AssetManager::RegisterTerrain(const TerrainRegistry& reg)
{
	mTerrainRegister[reg.ID] = reg;
	mTerrainIDLookUp[reg.Name] = reg.ID;
	IsRegisterDirty = true;
	FRACTURE_TRACE("Registering Terrain: {} ", reg.Path);
}

std::shared_ptr<Fracture::Terrain> Fracture::AssetManager::GetTerrain(const std::string& Name)
{
	{
		if (!IsTerrainLoaded(Name))
		{
			auto it = mTerrainRegister.find(mTerrainIDLookUp[Name]);
			if (it != mTerrainRegister.end())
			{ }
				//mTerrains[mTerrainIDLookUp[Name]] = AnimationClipLoader::LoadAnimationClip(mTerrainRegister[mTerrainIDLookUp[Name]].Path);
		}


		auto it = mTerrainIDLookUp.find(Name);
		if (it != mTerrainIDLookUp.end())
		{
			auto check_Terrain = mTerrains.find(it->second);
			if (check_Terrain != mTerrains.end())
			{
				return mTerrains[it->second];
			}
		};
	}
	FRACTURE_ERROR("Could not find Terrain: {}", Name);
	return nullptr;
}

std::shared_ptr<Fracture::Terrain> Fracture::AssetManager::GetTerrainByID(const Fracture::UUID& id)
{
	if (!IsTerrainLoaded(id))
	{
		auto it = mTerrainRegister.find(id);
		if (it != mTerrainRegister.end())
		{
			if (mTerrainRegister[id].Path.empty())
			{
				return nullptr;
			}
			
			//????? TO DO TERRAIN LOADER.

			FRACTURE_TRACE("Loaded Terrain: {}", mTerrainRegister[id].Path);
		}
		else
		{
			FRACTURE_ERROR("Terrain: {} , Is not Registered", id);
		}
	}
	else if (IsTerrainLoaded(id))
	{
		return mTerrains[id];
	}
	FRACTURE_ERROR("Could not find Terrain: {}", id);
	return nullptr;
}

void Fracture::AssetManager::OnAsyncLoadMesh(const std::shared_ptr<AsyncLoadMeshEvent>& evnt)
{
	if(!IsMeshLoaded(evnt->MeshID))
		AsyncLoadMeshByID(evnt->MeshID);
}

void Fracture::AssetManager::OnAsyncLoadTexture(const std::shared_ptr<AsyncLoadTextureEvent>& evnt)
{
	if (!IsTextureLoaded(evnt->TextureID))
	{
		AsyncLoadTextureByID(evnt->TextureID);
	}
}

void Fracture::AssetManager::OnAsyncLoadMaterial(const std::shared_ptr<AsyncLoadMaterialEvent>& evnt)
{
	if (!IsMaterialLoaded(evnt->MaterialID))
		AsyncLoadMaterialByID(evnt->MaterialID);
}

void Fracture::AssetManager::OnAsyncLoadandAttach(const std::shared_ptr<AsyncLoadMeshAndAttachEvent>& evnt)
{
	if (IsMeshLoaded(evnt->MeshID))
	{
		SceneManager::AddComponent<MeshComponent>(evnt->EntityID, evnt->MeshID,mMeshes[evnt->MeshID]->mMaterials);
		return;
	}

	auto it = mMeshRegister.find(evnt->MeshID);
	if (it != mMeshRegister.end())
	{
		LoadandAttachContext cntxt;
		cntxt.Entity = evnt->EntityID;
		cntxt.Mesh = it->first;
		mMeshesToLoadandAttach.push(cntxt);
	}
}

void Fracture::AssetManager::OnLoadTexture(const std::shared_ptr<LoadTextureEvent>& evnt)
{
	if (IsTextureLoaded(evnt->TextureID))
	{
		glDeleteTextures(1, &mTextures[evnt->TextureID]->Handle);
		mTextures[evnt->TextureID]->Description.format = mTextureRegister[evnt->TextureID].format;
		mTextures[evnt->TextureID]->Description.internalFormat = mTextureRegister[evnt->TextureID].internalFormat;
		mTextures[evnt->TextureID]->Description.Wrap = mTextureRegister[evnt->TextureID].Wrap;
		mTextures[evnt->TextureID]->Description.magFilter = mTextureRegister[evnt->TextureID].magFilter;
		mTextures[evnt->TextureID]->Description.minFilter = mTextureRegister[evnt->TextureID].minFilter;
		mTextures[evnt->TextureID]->Description.GenMinMaps = mTextureRegister[evnt->TextureID].GenMinMaps;
		mTextures[evnt->TextureID]->Description.formatType = mTextureRegister[evnt->TextureID].formatType;
		GraphicsDevice::Instance()->CreateTexture(mTextures[evnt->TextureID], mTextures[evnt->TextureID]->Description);
	}
	else
	{
		AsyncLoadTextureByID(evnt->TextureID);
	}
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

void Fracture::AssetManager::AsyncLoadTexture(const std::string& Name)
{
	auto it = mTextureIDLookUp.find(Name);
	if (it != mTextureIDLookUp.end())
	{
		mTexturesToLoad.push(mTextureRegister[it->second]);
	}
}

void Fracture::AssetManager::AsyncLoadTextureByID(const UUID& id)
{
	auto it = mTextureRegister.find(id);
	if (it != mTextureRegister.end())
	{
		mTexturesToLoad.push(mTextureRegister[it->first]);
	}
}

void Fracture::AssetManager::AsyncLoadMaterial(const std::string& Name)
{
	auto it = mMaterialIDLookUp.find(Name);
	if (it != mMaterialIDLookUp.end())
	{
		mMaterialToLoad.push(mMaterialRegister[it->second]);
	}
}

void Fracture::AssetManager::AsyncLoadMaterialByID(const UUID& id)
{
	auto it = mMaterialRegister.find(id);
	if (it != mMaterialRegister.end())
	{
		mMaterialToLoad.push(mMaterialRegister[it->first]);
	}
}

void Fracture::AssetManager::AsyncLoadAnimation(const std::string& Name)
{
	auto it = mAnimationIDLookUp.find(Name);
	if (it != mAnimationIDLookUp.end())
	{
		mAnimationsToLoad.push(mAnimationRegister[it->second]);
	}
}

void Fracture::AssetManager::AsyncLoadAnimationByID(const UUID& id)
{
	auto it = mAnimationRegister.find(id);
	if (it != mAnimationRegister.end())
	{
		mAnimationsToLoad.push(mAnimationRegister[it->first]);
	}
}

void Fracture::AssetManager::AddTexture(TextureCreationInfo& desc)
{	
	mTextures[desc.ID] = std::make_shared<Texture>(desc);
	GraphicsDevice::Instance()->CreateTexture(mTextures[desc.ID], desc);
	mLoadedTextures.push_back(desc.ID);
}

bool Fracture::AssetManager::IsMeshLoaded(const std::string& Name)
{
	return std::find(mLoadedMeshes.begin(), mLoadedMeshes.end(), mMeshIDLookUp[Name]) != mLoadedMeshes.end();
}

bool Fracture::AssetManager::IsMeshLoaded(const UUID& id)
{
	return std::find(mLoadedMeshes.begin(), mLoadedMeshes.end(), id) != mLoadedMeshes.end();
}

bool Fracture::AssetManager::IsTextureLoaded(const UUID& id)
{
	return std::find(mLoadedTextures.begin(), mLoadedTextures.end(), id) != mLoadedTextures.end();
}

bool Fracture::AssetManager::IsTextureLoaded(const std::string& Name)
{
	return std::find(mLoadedTextures.begin(), mLoadedTextures.end(), mTextureIDLookUp[Name]) != mLoadedTextures.end();
}

bool Fracture::AssetManager::IsMaterialLoaded(const UUID& id)
{
	return std::find(mLoadedMaterials.begin(), mLoadedMaterials.end(), id) != mLoadedMaterials.end();
}

bool Fracture::AssetManager::IsMaterialLoaded(const std::string& Name)
{
	return std::find(mLoadedMaterials.begin(), mLoadedMaterials.end(), mMaterialIDLookUp[Name]) != mLoadedMaterials.end();
}

bool Fracture::AssetManager::IsAnimationLoaded(const UUID& id)
{
	return std::find(mLoadedAnimations.begin(), mLoadedAnimations.end(), id) != mLoadedAnimations.end();
}

bool Fracture::AssetManager::IsAnimationLoaded(const std::string& Name)
{
	return std::find(mLoadedAnimations.begin(), mLoadedAnimations.end(), mAnimationIDLookUp[Name]) != mLoadedAnimations.end();
}

bool Fracture::AssetManager::IsTerrainLoaded(const UUID& Name)
{
	return std::find(mLoadedTerrains.begin(), mLoadedTerrains.end(), Name) != mLoadedTerrains.end();
}

bool Fracture::AssetManager::IsTerrainLoaded(const std::string& Name)
{
	return std::find(mLoadedTerrains.begin(), mLoadedTerrains.end(), mTerrainIDLookUp[Name]) != mLoadedTerrains.end();
}

Fracture::UUID Fracture::AssetManager::GetMeshID(const std::string& Name)
{
	auto it = mMeshIDLookUp.find(Name);
	if (it != mMeshIDLookUp.end())
	{
		return it->second;
	}
}

std::shared_ptr<Fracture::StaticMesh> Fracture::AssetManager::GetStaticMesh(const std::string& Name)
{
	{
		auto it = mMeshIDLookUp.find(Name);
		if (it != mMeshIDLookUp.end())
		{
			return mMeshes[it->second];
		}
	}
	return nullptr;
}

std::shared_ptr<Fracture::StaticMesh>  Fracture::AssetManager::GetStaticByIDMesh(const UUID& id)
{
	{
		auto it = mMeshes.find(id);
		if (it != mMeshes.end())
		{
			return mMeshes[id];
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

bool Fracture::AssetManager::HasTexturePath(const std::string& path)
{
	for (const auto& reg : mTextureRegister)
	{
		if (reg.second.Path == path)
			return true;
	}
	return false;
}

bool Fracture::AssetManager::HasMaterialPath(const std::string& path)
{
	for (const auto& reg : mMaterialRegister)
	{
		if (reg.second.Path == path)
			return true;
	}
	return false;
}

bool Fracture::AssetManager::HasTerrainPath(const std::string& path)
{
	for (const auto& reg : mTerrainRegister)
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
	IsRegisterDirty = true;
	FRACTURE_INFO("Registered Shader: {} {}", reg.ID, reg.Name);
}

int Fracture::AssetManager::CountUseCountMesh(UUID mesh)
{
	if (IsMeshLoaded(mesh))
	{
		return GetStaticByIDMesh(mesh).use_count();
	}

	return 0;
}

Fracture::AssetManager* Fracture::AssetManager::Instance()
{
	if (mInstance)
		return mInstance.get();

	mInstance = std::make_unique<AssetManager>();
	return mInstance.get();
}