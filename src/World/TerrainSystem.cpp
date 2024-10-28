#include "FracturePCH.h"
#include "TerrainSystem.h"
#include "SceneManager.h"
#include "Assets/AssetManager.h"
#include "Rendering/Texture.h"
#include "Rendering/Terrain.h"
#include "Rendering/GraphicsDevice.h"
#include "World/CameraSystem.h"
#include "Input/Input.h"
#include "Assets/ImagerLoader.h"
#include "stb/stb_image.h"
#include "Rendering/DebugRenderer.h"
#include "EventSystem/Eventbus.h"
#include "Rendering/Mesh.h"
#include <random>
#include "PlacementModifiers.h"
#include "Physics/Ray.h"

float smootherstep(float edge0, float edge1, float x) {
	// Scale, bias and saturate x to 0..1 range
	x = glm::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	// Evaluate polynomial
	return x * x * x * (x * (x * 6 - 15) + 10);
}

float EncodeIDAndWeight(int id, float weight) {
	// Ensure weight is in the range [0, 1)
	weight = std::clamp(weight, 0.0f, 0.9999f);
	return static_cast<float>(id) + weight;
}

std::pair<int, float> DecodeIDAndWeight(float encodedValue) {
	int id = static_cast<int>(encodedValue);
	float weight = encodedValue - static_cast<float>(id);
	if (weight >= 0.98f) weight = 1.0f;
	return { id, weight };
}

std::array<std::pair<int, float>, 4> sortIDsByWeight(std::array<std::pair<int, float>, 4>& idWeightPairs) {
	std::sort(idWeightPairs.begin(), idWeightPairs.end(), [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
		return a.second < b.second; // Sort in ascending order by weight
		});
	return idWeightPairs;
}

void Fracture::TerrainSystem::GenerateTerrain(Fracture::UUID entity)
{
	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);	
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return;

	{
		if (component->HasHeightMap)
		{			
			TextureCreationInfo desc;
			desc.ID = UUID();
			desc.TextureTarget = TextureTarget::Texture2D;
			desc.AttachmentTrgt = AttachmentTarget::Color;
			desc.format = TextureFormat::Red;
			desc.formatType = TextureFormatType::Float;
			desc.internalFormat = InternalFormat::R16F;
			desc.minFilter = TextureMinFilter::Linear;
			desc.magFilter = TextureMagFilter::Linear;
			desc.Wrap = TextureWrap::ClampToEdge;
			desc.Name = "HeightMap";
			desc.Width = (int)component->TerrianSizeX;
			desc.Height = (int)component->TerrianSizeY;			
			terrain->HeightMapData.resize(component->TerrianSizeY * component->TerrianSizeX, 0.0f);
			LoadHeightMap(component->HeightMapPath, terrain->HeightMapData);
			
			desc.texture_data = terrain->HeightMapData.data();
			component->HeightMapID = desc.ID;


			AssetManager::Instance()->AddTexture(desc);
		}
		else
		{
			// load height map texture
			TextureCreationInfo desc;
			desc.ID = UUID();
			desc.TextureTarget = TextureTarget::Texture2D;
			desc.AttachmentTrgt = AttachmentTarget::Color;
			desc.format = TextureFormat::Red;
			desc.formatType = TextureFormatType::Float;
			desc.internalFormat = InternalFormat::R16F;
			desc.minFilter = TextureMinFilter::Linear;
			desc.magFilter = TextureMagFilter::Linear;
			desc.Wrap = TextureWrap::ClampToEdge;
			terrain->HeightMapData.resize(component->TerrianSizeY * component->TerrianSizeX, 0.0f);
			component->HeightMapID = desc.ID;
			component->HasHeightMap = true;
			desc.Name = "GPUHeightMap";
			desc.Width = (int)component->TerrianSizeX;
			desc.Height = (int)component->TerrianSizeY;
			desc.texture_data = terrain->HeightMapData.data();
			AssetManager::Instance()->AddTexture(desc);
		}

		//GPU ROADMAP
		{
			// load height map texture
			TextureCreationInfo desc;
			desc.ID = UUID();
			desc.TextureTarget = TextureTarget::Texture2D;
			desc.AttachmentTrgt = AttachmentTarget::Color;
			desc.format = TextureFormat::Red;
			desc.formatType = TextureFormatType::Float;
			desc.internalFormat = InternalFormat::R16F;
			desc.minFilter = TextureMinFilter::Linear;
			desc.magFilter = TextureMagFilter::Linear;
			desc.Wrap = TextureWrap::ClampToEdge;
			component->RoadMapID = desc.ID;
			component->HasRoadsMap = true;
			desc.Name = "GPURoadMap";
			desc.Width = (int)component->TerrianSizeX;
			desc.Height = (int)component->TerrianSizeY;
			desc.f_data.resize(component->TerrianSizeY * component->TerrianSizeX, 0.0f);
			AssetManager::Instance()->AddTexture(desc);
		}

		//MixMap
		if (component->HasMixMap)
		{
			TextureCreationInfo mixmap_desc;
			mixmap_desc.ID = component->MixMapID;
			mixmap_desc.TextureTarget = TextureTarget::Texture2D;
			mixmap_desc.AttachmentTrgt = AttachmentTarget::Color;
			mixmap_desc.format = TextureFormat::RGBA;
			mixmap_desc.formatType = TextureFormatType::Float;
			mixmap_desc.internalFormat = InternalFormat::RGBA16F;
			mixmap_desc.minFilter = TextureMinFilter::Linear;
			mixmap_desc.magFilter = TextureMagFilter::Linear;
			mixmap_desc.Wrap = TextureWrap::ClampToEdge;
			mixmap_desc.Width = (int)component->TerrianSizeY;
			mixmap_desc.Height = (int)component->TerrianSizeX;
			mixmap_desc.Name = "GPUMixMap";			
			mixmap_desc.NoChannels = 4;
			mixmap_desc.f_data.resize(component->TerrianSizeX * component->TerrianSizeY * 4, 0.0f);
			LoadHeightMap(component->MixMapPath, mixmap_desc.f_data,4);
			AssetManager::Instance()->AddTexture(mixmap_desc);
		}
		else
		{
			TextureCreationInfo mixmap_desc;
			mixmap_desc.ID = UUID();
			mixmap_desc.TextureTarget = TextureTarget::Texture2D;
			mixmap_desc.AttachmentTrgt = AttachmentTarget::Color;
			mixmap_desc.format = TextureFormat::RGBA;
			mixmap_desc.formatType = TextureFormatType::Float;
			mixmap_desc.internalFormat = InternalFormat::RGBA16F;
			mixmap_desc.minFilter = TextureMinFilter::Linear;
			mixmap_desc.magFilter = TextureMagFilter::Linear;
			mixmap_desc.Wrap = TextureWrap::ClampToEdge;
			mixmap_desc.Width = (int)component->TerrianSizeY;
			mixmap_desc.Height = (int)component->TerrianSizeX;
			mixmap_desc.Name = "GPUMixMap";
			mixmap_desc.NoChannels = 4;
			component->MixMapID = mixmap_desc.ID;
			component->HasMixMap = true;
			AssetManager::Instance()->AddTexture(mixmap_desc);
		}

		//SplatMap
		{
			TextureCreationInfo splatmap_desc;
			splatmap_desc.ID = UUID();
			splatmap_desc.TextureTarget = TextureTarget::Texture2D;
			splatmap_desc.AttachmentTrgt = AttachmentTarget::Color;
			splatmap_desc.format = TextureFormat::RGBA;
			splatmap_desc.formatType = TextureFormatType::Float;
			splatmap_desc.internalFormat = InternalFormat::RGBA16F;
			splatmap_desc.minFilter = TextureMinFilter::Linear;
			splatmap_desc.magFilter = TextureMagFilter::Linear;
			splatmap_desc.Wrap = TextureWrap::ClampToEdge;
			splatmap_desc.Width = (int)component->TerrianSizeY;
			splatmap_desc.Height = (int)component->TerrianSizeX;
			splatmap_desc.Name = "GPUMixMap";
			splatmap_desc.f_data.resize(component->TerrianSizeX * component->TerrianSizeY * 4, 1.0f);
			component->IndexMapID = splatmap_desc.ID;
			AssetManager::Instance()->AddTexture(splatmap_desc);
		}
		//Diffuse Atlas
		{
			TextureCreationInfo diff_desc;
			diff_desc.ID = UUID();
			diff_desc.TextureTarget = TextureTarget::Texture2D;
			diff_desc.AttachmentTrgt = AttachmentTarget::Color;
			diff_desc.format = TextureFormat::RGB;
			diff_desc.formatType = TextureFormatType::UByte;
			diff_desc.internalFormat = InternalFormat::RGB32F;
			diff_desc.minFilter = TextureMinFilter::Linear;
			diff_desc.magFilter = TextureMagFilter::Linear;
			diff_desc.Wrap = TextureWrap::ClampToEdge;
			diff_desc.Width = 2048;
			diff_desc.Height = 2560;
			diff_desc.NoChannels = 3;
			diff_desc.Name = "GPUDiffuseAtlasMap";
			diff_desc.data.resize(diff_desc.Width * diff_desc.Height * diff_desc.NoChannels, 0);
			component->DiffuseTextureAtlasID =diff_desc.ID;
			component->HasDiffuseTextureAtlas = true;
			AssetManager::Instance()->AddTexture(diff_desc);
		}
		//NormalMap Atlas
		{
			TextureCreationInfo Norm_desc;
			Norm_desc.ID = UUID();
			Norm_desc.TextureTarget = TextureTarget::Texture2D;
			Norm_desc.AttachmentTrgt = AttachmentTarget::Color;
			Norm_desc.format = TextureFormat::RGB;
			Norm_desc.formatType = TextureFormatType::UByte;
			Norm_desc.internalFormat = InternalFormat::RGB32F;
			Norm_desc.minFilter = TextureMinFilter::Linear;
			Norm_desc.magFilter = TextureMagFilter::Linear;
			Norm_desc.Wrap = TextureWrap::ClampToEdge;
			Norm_desc.Width = 2048;
			Norm_desc.Height = 2560;
			Norm_desc.NoChannels = 3;
			Norm_desc.Name = "NormalAtlasMap";
			Norm_desc.data.resize(Norm_desc.Width * Norm_desc.Height * Norm_desc.NoChannels, 0);
			component->NormalTextureAtlasID = Norm_desc.ID;
			component->HasNormalTextureAtlas = true;
			AssetManager::Instance()->AddTexture(Norm_desc);
		}
		//CPU OutHeightMap
		{
			auto texture = std::make_shared<TerrainTextureMap>();
			texture->Width = component->TerrianSizeX;
			texture->Height = component->TerrianSizeY;
			texture->Channels = 1;
			texture->Name = "CPUOutputHeightMap";
			texture->Data.resize(texture->Width * texture->Height * texture->Channels, 0.0f);
			texture->IsDirty = true;
			mOutputHeightMaps[component->entity] = terrain->HeightMapData;
		}
		//Road Map
		{
			auto texture = std::make_shared<TerrainTextureMap>();
			texture->Width = component->TerrianSizeX;
			texture->Height = component->TerrianSizeY;
			texture->Channels = 1;
			texture->Name = "CPURoadMap";
			texture->Data.resize(texture->Width* texture->Height* texture->Channels, 0.0f);
			texture->IsDirty = true;
			mRoadMaps[component->entity] = texture;
		}
	}

	int width = component->TerrianSizeX;
	int height = component->TerrianSizeY;
	for (unsigned i = 0; i <= component->TerrianResolution - 1; i++)
		{
			for (unsigned j = 0; j <= component->TerrianResolution - 1; j++)
			{
				Vertex v1;
				v1.Position.x = (-width / 2.0f + width * i / (float)component->TerrianResolution); // v.x
				v1.Position.y = (0.0f); // v.y
				v1.Position.z = (-height / 2.0f + height * j / (float)component->TerrianResolution); // v.z
				v1.Uvs.x = (i / (float)component->TerrianResolution); // u
				v1.Uvs.y = (j / (float)component->TerrianResolution); // v
				terrain->Vertices.push_back(v1);

				Vertex v2;
				v2.Position.x = (-width / 2.0f + width * (i + 1) / (float)component->TerrianResolution); // v.x
				v2.Position.y = (0.0f); // v.y
				v2.Position.z = (-height / 2.0f + height * j / (float)component->TerrianResolution); // v.z
				v2.Uvs.x = (i + 1) / (float)component->TerrianResolution; // u
				v2.Uvs.y = (j / (float)component->TerrianResolution); // v
				terrain->Vertices.push_back(v2);

				Vertex v3;
				v3.Position.x = (-width / 2.0f + width * i / (float)component->TerrianResolution); // v.x
				v3.Position.y = (0.0f); // v.y
				v3.Position.z = (-height / 2.0f + height * (j + 1) / (float)component->TerrianResolution); // v.z
				v3.Uvs.x = (i / (float)component->TerrianResolution); // u
				v3.Uvs.y = ((j + 1) / (float)component->TerrianResolution); // v
				terrain->Vertices.push_back(v3);

				Vertex v4;
				v4.Position.x = (-width / 2.0f + width * (i + 1) / (float)component->TerrianResolution); // v.x
				v4.Position.y = (0.0f); // v.y
				v4.Position.z = (-height / 2.0f + height * (j + 1) / (float)component->TerrianResolution); // v.z
				v4.Uvs.x = ((i + 1) / (float)component->TerrianResolution); // u
				v4.Uvs.y = ((j + 1) / (float)component->TerrianResolution); // v
				terrain->Vertices.push_back(v4);
			}
		}
	
	VertexArrayCreationInfo info;
	info.Layout =
	{
		{ ShaderDataType::Float3,"aPos",0,true },
		{ ShaderDataType::Float3,"aNormal" ,0,true},
		{ ShaderDataType::Float2,"aUV" ,0,true}
	};

	GraphicsDevice::Instance()->CreateVertexArray(terrain->VAO, info);
	{
		BufferDescription b_desc;
		b_desc.data = terrain->Vertices.data();
		b_desc.bufferType = BufferType::ArrayBuffer;
		b_desc.size = sizeof(terrain->Vertices[0]) * terrain->Vertices.size();
		b_desc.usage = BufferUsage::Static;
		b_desc.Name = "Verticies";
		b_desc.IsPersistantlyMapped = true;
		b_desc.BufferAccessFlags = BufferAccess::ReadWrite;
		terrain->VBO_Buffer = std::make_shared<Buffer>();
		GraphicsDevice::Instance()->CreateBuffer(terrain->VBO_Buffer.get(), b_desc);
		GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(terrain->VAO, 0, sizeof(terrain->Vertices[0]), terrain->VBO_Buffer->RenderID);
	}
	
	GraphicsDevice::Instance()->VertexArray_BindAttributes(terrain->VAO, info);
	component->IsGenerated = true;


}

void Fracture::TerrainSystem::UpdateTerrain(Fracture::UUID entity)
{
	OPTICK_EVENT();
	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
	const auto& hierachy = SceneManager::GetComponent<HierachyComponent>(entity);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);
	if (component->IsGenerated)
	{
		
		if (mRoadMaps.find(component->entity) != mRoadMaps.end())
		{
			std::fill(mRoadMaps[component->entity]->Data.begin(), mRoadMaps[component->entity]->Data.end(), 0.0f);
		
			for (const auto& child : hierachy->Children)
			{

				if (SceneManager::HasComponent<SplineComponent>(child))
				{
					SplineModifier(entity, child);
				}
			}
		}

		if (component->HasRoadsMap && component->IsRoadMapDirty)
		{
			const auto& roadmap = AssetManager::GetTextureByID(component->RoadMapID);
			roadmap->Description.f_data = mRoadMaps[component->entity]->Data;

			GraphicsDevice::Instance()->UpdateFloatTexture(component->RoadMapID);
			component->IsRoadMapDirty = false;
		}			
		if (component->HasHeightMap && terrain->IsDirty)
		{

			auto  blendFunc = [&](float a, float b, float blendFactor) {
				// Calculate the height difference
				float HeightDifference = (b/component->TerrianMaxHeight) - a;
				if (b != 0)
				{
					a += HeightDifference;
				}				
				return a;
			};			

			std::vector<float> result(mOutputHeightMaps[component->entity].size());
			std::transform(mOutputHeightMaps[component->entity].begin(), mOutputHeightMaps[component->entity].end(), mRoadMaps[component->entity]->Data.begin(), result.begin(),
				[blendFunc](float a, float b) { return blendFunc(a, b, 0.5f); });


			GraphicsDevice::Instance()->UpdateTexture<float>(component->HeightMapID, result, 0, 0, component->TerrianSizeX, component->TerrianSizeY);
			terrain->HeightMapData = mOutputHeightMaps[component->entity];
			terrain->IsDirty = false;
		}
		if (component->HasMixMap && component->IsMixMapDirty)
		{
			GraphicsDevice::Instance()->UpdateFloatTexture(component->MixMapID);
			component->IsMixMapDirty = false;
		}
		
		if (component->IsAtlasDirty)
		{
			UpdateTerrainAtlas(component->entity);
			component->IsAtlasDirty = false;
		}
	}
}

void Fracture::TerrainSystem::OnInit()
{
	bvh = std::make_unique<BVHTree>();
}

void Fracture::TerrainSystem::OnUpdate()
{
	OPTICK_EVENT();
	const auto& terrains = SceneManager::GetAllComponents<TerrainComponent>();
	for (const auto& comp : terrains)
	{
		UpdateTerrain(comp->entity);

		for (const auto& layerId : comp->PlacementLayers)
		{
			if (mPlacementLayers.find(comp->entity) != mPlacementLayers.end())
			{
				const auto& layers = mPlacementLayers[comp->entity];
				int index = 0;
				for (const auto& layer : layers)
				{			
					UpdateLayer(comp->entity, index);
					if (mPlacementMaps.find(layer->PlacemenMapID) != mPlacementMaps.end())
					{
						auto& map = mPlacementMaps[layer->PlacemenMapID];
						UpdateDensityMap(layer->PlacemenMapID);
						
						if (map->IsDirty)
						{							
							if (layer->IsFootPrintDirty)
							{
								mPlacementPoints[layer->PlacemenMapID] = GenerateGridPoints(layer->FootPrint, glm::vec2(map->Width, map->Height));
								layer->IsFootPrintDirty = false;
							}
							map->IsDirty = false;
						}
					}
					index++;
				}
			}
		}	
	}
}

void Fracture::TerrainSystem::OnBeginFrame(RenderContext* context)
{
	OPTICK_EVENT();
	const auto& terrains = SceneManager::GetAllComponents<TerrainComponent>();
	for (const auto& component : terrains)
	{
		if (!component->IsGenerated)
			continue;

		if (mPlacementLayers.find(component->entity) == mPlacementLayers.end())
			continue;
		const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

		for (const auto& layer : mPlacementLayers[component->entity])
		{
			const auto& comp = SceneManager::GetComponent<MeshComponent>(layer->LayerID);
			if (!comp)
				continue;

			std::vector<glm::mat4> mBatch;
			CameraSystem cam_system;
			for (int i = 0; i < mPlacementPoints[layer->PlacemenMapID].size(); i++)
			{
				float offsetx = -component->TerrianSizeX / 2;
				float offsety = -component->TerrianSizeY / 2;

				int index = (int)mPlacementPoints[layer->PlacemenMapID][i].z * component->TerrianSizeX + (int)mPlacementPoints[layer->PlacemenMapID][i].x;
				
				glm::vec3 position = glm::vec3(mPlacementPoints[layer->PlacemenMapID][i].x + offsetx, 0, mPlacementPoints[layer->PlacemenMapID][i].z + offsety);
				position.y = mOutputHeightMaps[component->entity][(int)(int)mPlacementPoints[layer->PlacemenMapID][i].z * component->TerrianSizeX + (int)mPlacementPoints[layer->PlacemenMapID][i].x] * component->TerrianMaxHeight - component->TerrianYShift;
				//DebugRenderer::DrawSphere(position, 0.5f);
				if (index >= 0 && index < mDenistyMaps[layer->PlacemenMapID]->Data.size() && mDenistyMaps[layer->PlacemenMapID]->Data[index] > 0)
				{
					if (cam_system.IsPointInFrustum(*SceneManager::ActiveCamera(), position))
					{
						float distance = glm::distance(SceneManager::ActiveCamera()->Position, position);
						if (distance < layer->DrawDistance)
						{
							glm::mat4 translation = glm::translate(position);
							glm::mat4 scale = glm::scale(layer->Scale);
							glm::mat4 rotate = glm::toMat4(glm::normalize(glm::quat(layer->Rotation)));
							glm::mat4 transform = translation * rotate * scale;
							mBatch.push_back(transform);
						}
					}
				}
			}

			const auto& mesh = AssetManager::GetStaticByIDMesh(comp->Mesh);
			for (auto& sub : mesh->SubMeshes)
			{
				context->AddToBatch(&sub, comp->Mesh, comp->Materials[sub.MaterialIndex], mBatch);
			}

		}
	}
}

void Fracture::TerrainSystem::OnEndFrame()
{		
		
}

void Fracture::TerrainSystem::OnDebugDraw()
{	
	if (bvh->root)
	{
		DrawNode(bvh->root.get());
	}

	for (auto& ray : mRays)
	{
		DebugRenderer::DrawLine(ray.start,ray.finish);	
	}
	for (auto& p : mHitpoints)
	{
		DebugRenderer::DrawSphere(p,1.0f);		
	}
}

void Fracture::TerrainSystem::OnSave(const std::string& folder)
{
	const auto& terrains = SceneManager::GetAllComponents<TerrainComponent>();
	for (const auto& component : terrains)
	{
		SaveHeightMap(folder, component->entity);
	}
}

void Fracture::TerrainSystem::OnLoad()
{
	const auto& terrains = SceneManager::GetAllComponents<TerrainComponent>();
	for (const auto& component : terrains)
	{
		if (component->HasHeightMap && !component->IsGenerated)
		{
			AssetManager::Instance()->mTerrains[component->TerrainID] = std::make_shared<Terrain>(component->TerrainID);
			AssetManager::Instance()->mLoadedTerrains.push_back(component->TerrainID);
			GenerateTerrain(component->entity);
			UpdateTerrainAtlas(component->entity);
		}
	}
}

void Fracture::TerrainSystem::DrawNode(BVHNode* node)
{
	if (!node)return;

	if (node->Depth == DrawDepthFrom) {

		DebugRenderer::DrawAABB(node->Bounds);
	
	}
	DrawNode(node->Left.get());
	DrawNode(node->Right.get());
}

void Fracture::TerrainSystem::ApplyBrush(BrushParams params)
{
	OPTICK_EVENT();
	switch (EditMode)
	{
	case TerrainEditModeOptions::Selection:
	{
		break;
	}
	case TerrainEditModeOptions::Sculpting:
	{
		const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
		const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

		if (!terrain)
			return;

		const int numThreads = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;
		std::mutex mtx;
		
		auto brushFunc = [&](int startx, int endx, int starty, int endy,int heightX, int heightY) {			
				for (int y = starty; y < endy; ++y) {
					for (int x = startx; x < endx; ++x) {
						// Calculate the distance from the brush center
						float distance = std::sqrt((x - heightX) * (x - heightX) + (y - heightY) * (y - heightY));
						// Apply the influence if within the brush radius
						if (distance < MainBrush.radius) {							
							float influence = glm::exp(-MainBrush.Falloff * distance) * MainBrush.strength;
							float normalizedInfluence = influence / component->TerrianMaxHeight;
							int index = y * component->TerrianSizeX + x;						
							switch (MainBrush.BrushType) {
							case TerrainBrush::BrushTypeOptions::RAISE:
								mOutputHeightMaps[component->entity][index] = std::clamp(mOutputHeightMaps[component->entity][index] + normalizedInfluence, -1.0f, 1.0f);
								break;
							case TerrainBrush::BrushTypeOptions::LOWER:
								mOutputHeightMaps[component->entity][index] = std::clamp(mOutputHeightMaps[component->entity][index] - normalizedInfluence, -1.0f, 1.0f);
								break;
							case TerrainBrush::BrushTypeOptions::FLATTEN:
								mOutputHeightMaps[component->entity][index] = glm::mix(mOutputHeightMaps[component->entity][index], params.centre.y, influence);
								break;
							}
						}
					}
				}
		};

		float brushRadiusSq = MainBrush.radius * MainBrush.radius;
		// Calculate normalized coordinates
		float normalizedX = static_cast<float>(params.centre.x) / static_cast<float>(component->TerrianSizeX) + 0.5f;
		float normalizedY = static_cast<float>(params.centre.z) / static_cast<float>(component->TerrianSizeY) + 0.5f;

		int heightMapX = static_cast<int>(normalizedX * component->TerrianSizeX);
		int heightMapY = static_cast<int>(normalizedY * component->TerrianSizeY);


		int startX = std::max(0, static_cast<int>(heightMapX - MainBrush.radius));
		int endX = std::min(component->TerrianSizeX, static_cast<int>(heightMapX + MainBrush.radius));
		int startY = std::max(0, static_cast<int>(heightMapY - MainBrush.radius));
		int endY = std::min(component->TerrianSizeY, static_cast<int>(heightMapY + MainBrush.radius));

		
		int chunkHeight = component->TerrianSizeY / numThreads;
		for (int i = 0; i < numThreads; ++i) {
			int startY = i * chunkHeight;
			int endY = (i == numThreads - 1) ? component->TerrianSizeY : startY + chunkHeight;
			threads.emplace_back(brushFunc, 0, component->TerrianSizeX, startY, endY,heightMapX,heightMapY);
		}

		for (auto& thread : threads) {
			thread.join();
		}

		//bvh->update(terrain->Vertices, terrain->Indices);

		terrain->IsDirty = true;
		break;
	}
	case TerrainEditModeOptions::Placement:
	{
		if (mCurrentPlacementMapForEdit)
		{
			const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
			const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);
			auto& layer = mCurrentPlacementMapForEdit;
			
			// Calculate normalized coordinates
			float normalizedX = static_cast<float>(params.centre.x) / static_cast<float>(layer->Width) + 0.5f;
			float normalizedY = static_cast<float>(params.centre.z) / static_cast<float>(layer->Height) + 0.5f;

			int heightMapX = static_cast<int>(normalizedX * layer->Width);
			int heightMapY = static_cast<int>(normalizedY * layer->Height);

			int startX = std::max(0, static_cast<int>(heightMapX - MainBrush.radius));
			int endX = std::min(component->TerrianSizeX, static_cast<int>(heightMapX + MainBrush.radius));

			int startY = std::max(0, static_cast<int>(heightMapY - MainBrush.radius));
			int endY = std::min(component->TerrianSizeY, static_cast<int>(heightMapY + MainBrush.radius));

			for (int y = startY; y < endY; ++y) {
				for (int x = startX; x < endX; ++x) {
					// Calculate the distance from the brush center
					float distance = std::sqrt((x - heightMapX) * (x - heightMapX) + (y - heightMapY) * (y - heightMapY));
					// Apply the influence if within the brush radius
					if (distance < MainBrush.radius) {
						float influence = (1.0f - (distance / MainBrush.radius)) * 1.0f;
						switch (MainBrush.BrushType) {
						case TerrainBrush::BrushTypeOptions::RAISE:
							layer->Data[y * layer->Width + x] = 1;
							layer->IsDirty = true;
							break;
						case TerrainBrush::BrushTypeOptions::LOWER:
							layer->Data[y * layer->Width + x] = 0;
							layer->IsDirty = true;
							break;
						case TerrainBrush::BrushTypeOptions::FLATTEN:
							break;
						}
					}
				}
			}
			terrain->IsDirty = true;
		}
		break;
	}
	case TerrainEditModeOptions::Painting:
	{
		const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
		const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);
		if (component->TerrainTextures.empty())
			break;

		const auto& mixmap = AssetManager::GetTextureByID(component->MixMapID);	
		const auto& splatmap = AssetManager::GetTextureByID(component->IndexMapID);
		if (!mixmap)
			return;

		// Calculate normalized coordinates
		float normalizedX = static_cast<float>(params.centre.x) / static_cast<float>(component->TerrianSizeX) + 0.5f;
		float normalizedY = static_cast<float>(params.centre.z) / static_cast<float>(component->TerrianSizeY) + 0.5f;

		int heightMapX = static_cast<int>(normalizedX * component->TerrianSizeX);
		int heightMapY = static_cast<int>(normalizedY * component->TerrianSizeY);

		int startX = std::max(0, static_cast<int>(heightMapX - MainBrush.radius));
		int endX = std::min(component->TerrianSizeX, static_cast<int>(heightMapX + MainBrush.radius));

		int startY = std::max(0, static_cast<int>(heightMapY - MainBrush.radius));
		int endY = std::min(component->TerrianSizeY, static_cast<int>(heightMapY + MainBrush.radius));

		//FRACTURE_INFO("data size {}", mixmap->Description.f_data.size());
		for (int y = startY; y < endY; ++y) {
			for (int x = startX; x < endX; ++x) {
				// Calculate the distance from the brush center
				float distance = std::sqrt((x - heightMapX) * (x - heightMapX) + (y - heightMapY) * (y - heightMapY));
				// Apply the influence if within the brush radius

				
				//int index = (y * component->TerrianSizeX + x) * 3;					
				if (distance < MainBrush.radius) {
					float influence = glm::exp(-MainBrush.Falloff * distance) * MainBrush.strength;
					float normalizedInfluence = influence;// 1.0f - (distance / MainBrush.radius) * (distance / MainBrush.radius);
					

					int idN = params.TextureIndex; // New Texture ID being painted.						
					float TextureWeight = component->TerrainTextures[params.TextureIndex].Weight;
					int indexR = y * component->TerrianSizeX * 4 + x * 4;
					int indexG = indexR + 1;
					int indexB = indexR + 2;
					int indexA = indexR + 3;

					if (indexR >= 0 && indexA < mixmap->Description.f_data.size()) {
						switch (MainBrush.BrushType) {
						case TerrainBrush::BrushTypeOptions::RAISE:
						{			
							
							float weightInfluence = std::clamp(normalizedInfluence, 0.0f, 1.0f);
							if (idN == 0)
							{
								mixmap->Description.f_data[indexR] = std::clamp(mixmap->Description.f_data[indexR] + weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexG] = std::clamp(mixmap->Description.f_data[indexG] - weightInfluence, 0.0f, 1.0f);	
								mixmap->Description.f_data[indexB] = std::clamp(mixmap->Description.f_data[indexB] - weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexA] = std::clamp(mixmap->Description.f_data[indexA] - weightInfluence, 0.0f, 1.0f);					
							}
							if (idN == 1)
							{
								mixmap->Description.f_data[indexR] = std::clamp(mixmap->Description.f_data[indexR] - weightInfluence , 0.0f, 1.0f);
								mixmap->Description.f_data[indexG] = std::clamp(mixmap->Description.f_data[indexG] + weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexB] = std::clamp(mixmap->Description.f_data[indexB] - weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexA] = std::clamp(mixmap->Description.f_data[indexA] - weightInfluence, 0.0f, 1.0f);							}
							if (idN == 2)
							{
								mixmap->Description.f_data[indexR] =std::clamp(mixmap->Description.f_data[indexR] - weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexG] =std::clamp(mixmap->Description.f_data[indexG] - weightInfluence , 0.0f, 1.0f);
								mixmap->Description.f_data[indexB] =std::clamp(mixmap->Description.f_data[indexB] + weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexA] = std::clamp(mixmap->Description.f_data[indexA] - weightInfluence , 0.0f, 1.0f);
							}
							if (idN == 3)
							{
								mixmap->Description.f_data[indexR] = std::clamp(mixmap->Description.f_data[indexR] - weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexG] = std::clamp(mixmap->Description.f_data[indexG] - weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexB] = std::clamp(mixmap->Description.f_data[indexB] - weightInfluence, 0.0f, 1.0f);
								mixmap->Description.f_data[indexA] = std::clamp(mixmap->Description.f_data[indexA] + weightInfluence, 0.0f, 1.0f);
							}




							component->IsMixMapDirty = true;
							break;
						}
						case TerrainBrush::BrushTypeOptions::LOWER:
						{
							//mixmap->Description.f_data[index] = std::clamp(mixmap->Description.f_data[index] - normalizedInfluence, 0.0f, 1.0f);
							//mixmap->Description.f_data[index + 1] = 0;
							component->IsMixMapDirty = true;
							break;
						}
						case TerrainBrush::BrushTypeOptions::FLATTEN:
							break;
						}
					}
				}
			}
		}

		break;
	}
	}
	
}

void Fracture::TerrainSystem::AddNewDensityGrid(Fracture::UUID entity)
{
	/*
	if (!SceneManager::HasComponent<TerrainComponent>(entity))
		return;

	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);

	auto grid = std::make_shared<DensityGrid>();
	grid->TerrainComponent = entity;
	grid->GriseSizeNoPoints.x = component->TerrianSizeX / (int)grid->GriseSizePx.x;
	grid->GriseSizeNoPoints.y = component->TerrianSizeY / (int)grid->GriseSizePx.y;
	
	grid->GridCellPositions.resize(grid->GriseSizeNoPoints.y);
	for (int i = 0; i < grid->GriseSizeNoPoints.y; ++i)
		grid->GridCellPositions[i].resize(grid->GriseSizeNoPoints.x);

	grid->IsCellAvailable.resize(grid->GriseSizeNoPoints.y);
	for (int i = 0; i < grid->GriseSizeNoPoints.y; ++i)
		grid->IsCellAvailable[i].resize(grid->GriseSizeNoPoints.x);
	

	for (int i = 0; i < grid->GriseSizeNoPoints.y; i++)
	{
		for (int j = 0; j < grid->GriseSizeNoPoints.x; j++)
		{			
			grid->GridCellPositions[i][j].x = (-component->TerrianSizeX / 2.0f + j);// +(grid->GriseSizePx.x * j);
			grid->GridCellPositions[i][j].y = 1.0f;
			grid->GridCellPositions[i][j].z = (-component->TerrianSizeY / 2.0f + i);// +(grid->GriseSizePx.y * i);
		}
	}
	mDensityGrids[entity].push_back(std::move(grid));
	SetCurrentGrid(entity, mDensityGrids[entity].size() - 1);
	*/
}

void Fracture::TerrainSystem::UpdateLayer(Fracture::UUID entity, int index)
{
	OPTICK_EVENT();
	if (mPlacementLayers.find(entity) != mPlacementLayers.end())
	{
		if (!mPlacementLayers[entity][index]->Dirty)
			return;	
		
		auto& layer = mPlacementLayers[entity][index];

		if (mPlacementMaps.find(layer->PlacemenMapID) == mPlacementMaps.end())
			return;

		auto& map = mPlacementMaps[layer->PlacemenMapID];
		PlacementContext cntxt;
		cntxt.height = map->Height;;
		cntxt.width = map->Width;
		cntxt.PlacementMapID = layer->PlacemenMapID;
		cntxt._system = this;
		mModifiedPlacementMaps[layer->PlacemenMapID].resize(cntxt.height * cntxt.width);
		mModifiedPlacementMaps[layer->PlacemenMapID] = map->Data;

		for (const auto& m : layer->Modifiers)
		{
			if (!m->Enabled)
				continue;
			m->Modify(cntxt, mModifiedPlacementMaps[layer->PlacemenMapID]);
		}
	}
}

void Fracture::TerrainSystem::AddNewPlacementLayer(Fracture::UUID entity)
{
	if (!IsTerrainSubmittedForEdit)
		return;

	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
	auto layer = std::make_shared<PlacementLayer>();

	auto texture = std::make_shared<TerrainTextureMap>();
	texture->Width = component->TerrianSizeX;
	texture->Height = component->TerrianSizeY;
	texture->Channels = 1;
	texture->Name = "NewPlacementTexture";
	texture->Data.resize(texture->Width * texture->Height * texture->Channels, 0.0f);
	texture->IsDirty = true;
	layer->PlacemenMapID = texture->ID;

	auto density_texture = std::make_shared<TerrainTextureMap>();
	density_texture->Width = component->TerrianSizeX;
	density_texture->Height = component->TerrianSizeY;
	density_texture->Channels = 1;
	density_texture->Name = "NewDensityTexture";
	density_texture->Data.resize(density_texture->Width * density_texture->Height * density_texture->Channels, 1.0f);

	mModifiedPlacementMaps[texture->ID].resize(texture->Width * texture->Height * texture->Channels);
		
	mDenistyMaps[texture->ID] = density_texture;
	mPlacementMaps[texture->ID] = texture;

	component->PlacementLayers.push_back(layer->LayerID);
	mPlacementLayers[entity].push_back(layer);
}

void Fracture::TerrainSystem::UpdateDensityMap(Fracture::UUID placementID)
{
	OPTICK_EVENT();
	if (mPlacementMaps.find(placementID) == mPlacementMaps.end())
		return;

	if (mDenistyMaps.find(placementID) == mDenistyMaps.end())
		return;

	if (mModifiedPlacementMaps.find(placementID) == mModifiedPlacementMaps.end())
		return;

	mDenistyMaps[placementID]->Data = mModifiedPlacementMaps[placementID];
}

void Fracture::TerrainSystem::UpdateTerrainAtlas(Fracture::UUID entity)
{
	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);
	const auto& DiffAtlastexture = AssetManager::Instance()->GetTextureByID(component->DiffuseTextureAtlasID);
	const auto& NormAtlastexture = AssetManager::Instance()->GetTextureByID(component->NormalTextureAtlasID);
	int index = 0;
	int atlasWidth = DiffAtlastexture->Description.Width;
	int atlasHeight = DiffAtlastexture->Description.Height;
	int patchSize = component->AtlasPerTextureSize;
	int numChannels = DiffAtlastexture->Description.NoChannels;
	int maxWidth = atlasWidth / patchSize;
	int maxHeight = atlasHeight / patchSize;

	//Update Diffuse Textures
	for (int yIndex = 0;  yIndex < maxHeight; ++yIndex) {
		for (int xIndex = 0; xIndex < maxWidth; ++xIndex) {
			if (index < component->TerrainTextures.size()) {
				if (component->TerrainTextures[index].HasDiffuse)
				{
					const auto& current_texture = AssetManager::Instance()->GetTextureByID(component->TerrainTextures[index].DiffuseTexture);
					if (current_texture)
					{
						int xOffset = xIndex * patchSize;
						int yOffset = yIndex * patchSize;

						for (int y = 0; y < patchSize; y++)
						{
							for (int x = 0; x < patchSize; x++)
							{
								int textureWidth = current_texture->Description.Width;
								int textureHeight = current_texture->Description.Height;
								// Compute the corresponding source coordinates
								int srcX = std::min(static_cast<int>(x * (textureWidth / static_cast<float>(patchSize))), textureWidth - 1);
								int srcY = std::min(static_cast<int>(y * (textureHeight / static_cast<float>(patchSize))), textureHeight - 1);


								for (int c = 0; c < numChannels; ++c)
								{
									int atlasIndex = ((yOffset + y) * atlasWidth + (xOffset + x)) * numChannels + c;
									int textureIndex = (srcY * textureWidth + srcX) * numChannels + c;
									DiffAtlastexture->Description.data[atlasIndex] = current_texture->Description.data[textureIndex];
								}
							}
						}
						index++;
					}
				}
			}
		}
	}

	index = 0;
	//Update Normal Textures
	for (int yIndex = 0; yIndex < maxHeight; ++yIndex) {
		for (int xIndex = 0; xIndex < maxWidth; ++xIndex) {
			if (index < component->TerrainTextures.size()) {
				if (component->TerrainTextures[index].HasNormal)
				{
					const auto& current_texture = AssetManager::Instance()->GetTextureByID(component->TerrainTextures[index].NormalTexture);
					if (current_texture)
					{
						int xOffset = xIndex * patchSize;
						int yOffset = yIndex * patchSize;

						for (int y = 0; y < patchSize; y++)
						{
							for (int x = 0; x < patchSize; x++)
							{
								int textureWidth = current_texture->Description.Width;
								int textureHeight = current_texture->Description.Height;
								// Compute the corresponding source coordinates
								int srcX = std::min(static_cast<int>(x * (textureWidth / static_cast<float>(patchSize))), textureWidth - 1);
								int srcY = std::min(static_cast<int>(y * (textureHeight / static_cast<float>(patchSize))), textureHeight - 1);


								for (int c = 0; c < numChannels; ++c)
								{
									int atlasIndex = ((yOffset + y) * atlasWidth + (xOffset + x)) * numChannels + c;
									int textureIndex = (srcY * textureWidth + srcX) * numChannels + c;
									NormAtlastexture->Description.data[atlasIndex] = current_texture->Description.data[textureIndex];
								}
							}
						}
						index++;
					}
				}
			}
		}
	}

	GraphicsDevice::Instance()->UpdateTexture<uint8_t>(component->DiffuseTextureAtlasID, DiffAtlastexture->Description.data,0,0, DiffAtlastexture->Description.Width, DiffAtlastexture->Description.Height);
	GraphicsDevice::Instance()->UpdateTexture<uint8_t>(component->NormalTextureAtlasID, NormAtlastexture->Description.data,0,0, NormAtlastexture->Description.Width, NormAtlastexture->Description.Height);
}

void Fracture::TerrainSystem::SetCurrentPlacementMapForEdit(Fracture::UUID mapID)
{
	if (mPlacementMaps.find(mapID) == mPlacementMaps.end())
		return;

	mCurrentPlacementMapForEdit = mPlacementMaps[mapID].get();
}

void Fracture::TerrainSystem::SetCurrentPlacementLayerForEdit(Fracture::UUID entityID,int index)
{
	if (mPlacementLayers.find(entityID) == mPlacementLayers.end())
		return;

	mCurrentPlacementLayerForEdit = mPlacementLayers[entityID][index].get();
}

void Fracture::TerrainSystem::ClearGridSelection()
{

}

void Fracture::TerrainSystem::ClearPlacementMapFromEdit()
{
	mCurrentPlacementMapForEdit = nullptr;
}

void Fracture::TerrainSystem::AttachPlacementLayerToDensityGid(Fracture::UUID layer, Fracture::UUID grid)
{
}

glm::vec3 Fracture::TerrainSystem::AddJitterToVector(const glm::vec3& vector, float jitterAmount)
{
	// Initialize a random number generator
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(-jitterAmount, jitterAmount);

	// Create a jittered vector
	glm::vec3 jitteredVector = vector;
	jitteredVector.x += dist(rng);
	jitteredVector.z += dist(rng);

	return jitteredVector;
}

bool Fracture::TerrainSystem::IsCandidateValid(glm::vec3 candidate, glm::vec2 regionSize, float cellSize, float footprint, std::vector<std::vector<int>>& grid, std::vector<glm::vec3>& points)
{
	float xMin = 0;
	float yMin = 0;
	float xMax = regionSize.x;
	float yMax = regionSize.y;
	if (candidate.x >= xMin && candidate.x < xMax && candidate.z >= yMin && candidate.z < yMax)
	{
		int cellX = (int)candidate.x / cellSize;
		int cellY = (int)candidate.z / cellSize;
		int searchStartX = std::max(0, cellX - 2);
		int searchEndX = std::min(cellX + 2, (int)grid.size() - 1);
		int searchStartY = std::max(0, cellY - 2);
		int searchEndY = std::min(cellY + 2, (int)grid[0].size() - 1);
		for (int x = searchStartX; x <= searchEndX; x++)
		{
			for (int y = searchStartY; y <= searchEndY; y++)
			{
				int pointIndex = grid[x][y] - 1;
				if (pointIndex != -1)
				{
					
					float sqrDst = sqrMagnitude((candidate - points[pointIndex]));
					if (sqrDst < footprint * footprint)
					{
						return false;
					}
				}

			}


		}
		return true;
	}

	return false;
}

std::vector<glm::vec3> Fracture::TerrainSystem::GenerateGridPoints(float footprint, glm::vec2 regionSize, int NoOfSamples)
{
	OPTICK_EVENT();
	float cellSize = footprint / std::sqrt(2);
	int gridSizeX = (int)std::ceil( regionSize.x /cellSize);
	int gridSizeY = (int)std::ceil(regionSize.y / cellSize);

	std::vector<std::vector<int>> grid;
	grid.resize(gridSizeY);
	for (int i = 0; i < gridSizeY; i++)
		grid[i].resize(gridSizeX);

	std::vector<glm::vec3> points;
	std::vector<glm::vec3> spawnPoints;
	spawnPoints.push_back(glm::vec3(regionSize.x / 2,0, regionSize.y/2));

	while (!spawnPoints.empty())
	{
		int spawnIndex = (int)RandomRange(0,spawnPoints.size());
		glm::vec2 spawnCentre = glm::vec2(spawnPoints[spawnIndex].x, spawnPoints[spawnIndex].z);
		bool candidateAccepted = false;

		for (int i = 0; i < NoOfSamples; i++)
		{
			float rand = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
			float angle = rand * glm::pi<float>() * 2;
	
			glm::vec2 dir = glm::vec2(glm::sin(angle), glm::cosh(angle));
			//glm::vec2 candidate = spawnCentre + dir * RandomRange(footprint, 2 * footprint);
			glm::vec2 candidate = { spawnCentre.x + footprint * std::cos(angle), spawnCentre.y + footprint * std::sin(angle) };

			glm::vec3 spawn_C = glm::vec3(candidate.x, 0, candidate.y);
			if (IsCandidateValid(spawn_C,regionSize,cellSize,footprint, grid,points))
			{				
				points.push_back(spawn_C);
				spawnPoints.push_back(spawn_C);
				grid[(int)(candidate.x / cellSize)][(int)(candidate.y / cellSize)] = points.size();
				candidateAccepted = true;
				break;
			}
		}
		if (!candidateAccepted)
		{
			spawnPoints.erase(spawnPoints.begin() + spawnIndex);
		}

	}
	return points;
}

void Fracture::TerrainSystem::SplineModifier(Fracture::UUID entity, Fracture::UUID splineID)
{
	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);
	const auto& spline = SceneManager::GetComponent<SplineComponent>(splineID);
	if (!spline)
		return;

	int width = component->TerrianSizeX;
	int height = component->TerrianSizeY;
	int sampleCount = 300;
	float radius = 0.1f;
	float falloff = 1.0f;
	float falloffWidth = 1.0f;


	if (spline->UseWidth)
	{
		radius = spline->Width;
	}
	if (spline->UseFalloff)
	{
		falloff = spline->Falloff;
		falloffWidth = spline->FalloffWidth;
	}

	if (mRoadMaps.find(component->entity) == mRoadMaps.end())
		return;

	const auto& roadmap = mRoadMaps[component->entity];
	for (int i = 0; i < sampleCount; ++i)
	{
		// Apply influence in a rectangular area around the spline point
	
		if (!spline->Spline.Knots.empty())
		{
			float t = float(i) / (sampleCount - 1);
			glm::vec3 point = spline->Spline.CatmullRomSpline(t);
			// Calculate normalized coordinates
			float normalizedX = static_cast<float>(point.x) / static_cast<float>(width) + 0.5f;
			float normalizedY = static_cast<float>(point.z) / static_cast<float>(height) + 0.5f;

			int centerX = static_cast<int>(normalizedX * width);
			int centerY = static_cast<int>(normalizedY * height);

			// Apply falloff
			int totalarea = radius + falloffWidth;
			for (int offsetX = -totalarea; offsetX <= totalarea; ++offsetX) {
				for (int offsetY = -totalarea; offsetY <= totalarea; ++offsetY) {

					int x = centerX + offsetX;
					int y = centerY + offsetY;

					if (x >= 0 && x < width && y >= 0 && y < height) {
						int index = y * width + x;
						float distance = sqrt(offsetX * offsetX + offsetY * offsetY);

						// Compute the falloff weight
						float falloffWeight = 0.0f;
						if (distance <= radius) {
							falloffWeight = 1.0f;
						}
						else if (distance <= radius + falloffWidth)
						{
							//float normalizedDistance = (distance - radius) / falloffWidth;
							//falloffWeight = std::max(0.0f, 1.0f - normalizedDistance);

							float normalizedDistance = (distance - radius) / falloffWidth;
							falloffWeight = std::max(0.0f, 1.0f - pow(normalizedDistance, falloff));
						}
						roadmap->Data[index] = glm::mix(roadmap->Data[index], point.y, falloffWeight);
					}
				}
			}
		}
	}
	component->IsRoadMapDirty = true;
	terrain->IsDirty = true;
	spline->IsDirty = false;
}

void Fracture::TerrainSystem::SaveHeightMap(const std::string& path, Fracture::UUID entity)
{
	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	int size = component->TerrianSizeX * component->TerrianSizeY;
	if (component->IsGenerated)
	{
		std::string Outpath = path + std::to_string(component->entity) + ".Heightmap";
		FILE* f = fopen(Outpath.c_str(), "wb");


		if (!f)
		{
			FRACTURE_ERROR("Failed to save Height Map: {}", path);
		}


		Fracture::TextureFileHeader header;
		header.magicValue = 0;
		header.ID = terrain->ID;
		header.Width = component->TerrianSizeX;
		header.Height = component->TerrianSizeY;
		header.Channels = 1;
		header.Count = terrain->HeightMapData.size();
		header.DataSize = ((int64_t)component->TerrianSizeX * (int64_t)component->TerrianSizeY * 1);

		fwrite(&header, sizeof(header), 1, f);
		fwrite(&terrain->HeightMapData[0], sizeof(terrain->HeightMapData[0]), terrain->HeightMapData.size(), f);
		fclose(f);

		component->HasHeightMap = true;
		component->HeightMapPath = Outpath;
	}
	if(component->HasMixMap)
	{
		const auto& mixmap = AssetManager::GetTextureByID(component->MixMapID);
		std::string Outpath = path + std::to_string(component->MixMapID) + ".Mixmap";
		FILE* f = fopen(Outpath.c_str(), "wb");
		if (!f)
		{
			FRACTURE_ERROR("Failed to save Mix Map: {}", Outpath);
		}
		Fracture::TextureFileHeader header;
		header.magicValue = 0;
		header.ID = terrain->ID;
		header.Width = component->TerrianSizeX;
		header.Height = component->TerrianSizeY;
		header.Channels = 4;
		header.Count = mixmap->Description.f_data.size();
		header.DataSize = ((int64_t)component->TerrianSizeX * (int64_t)component->TerrianSizeY * 4);

		fwrite(&header, sizeof(header), 1, f);
		fwrite(&mixmap->Description.f_data[0], sizeof(mixmap->Description.f_data[0]), mixmap->Description.f_data.size(), f);
		fclose(f);
		
		component->HasMixMap = true;
		component->MixMapPath = Outpath;
	}
}

void Fracture::TerrainSystem::LoadHeightMap(const std::string& path, std::vector<float>& mapData, int NoChannels)
{
	FILE* f = fopen(path.c_str(), "rb");
	if (!f) {
		std::cerr << "Failed to load Map: " << path << std::endl;
		return;
	}

	Fracture::TextureFileHeader header;
	fread(&header, sizeof(header), 1, f);


	if (header.Channels != NoChannels) {
		std::cerr << "Unexpected number of channels: " << header.Channels << std::endl;
		fclose(f);
		return;
	}

	std::vector<float> buffer(header.Count);
	fread(buffer.data(), sizeof(float), header.Count, f);
	fclose(f);

	int width = header.Width;
	int height = header.Height;
	mapData.resize(width * height * header.Channels,0.0f);

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int rawIndex = i * width * header.Channels + j * header.Channels;
			// Populate all channels (RGBA)
			mapData[rawIndex + 0] =  static_cast<float>(buffer[rawIndex + 0]); // Red channel
			mapData[rawIndex + 1] =  static_cast<float>(buffer[rawIndex + 1]); // Green channel
			mapData[rawIndex + 2] =  static_cast<float>(buffer[rawIndex + 2]); // Blue channel
			mapData[rawIndex + 3] =  static_cast<float>(buffer[rawIndex + 3]); // Alpha channel
		}
	}
}

void Fracture::TerrainSystem::SubmitTerrainForEditing(Fracture::UUID entity)
{
	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return;

	Terrain_Width = component->TerrianSizeX;
	Terrain_Height = component->TerrianSizeY;
	OriginalTerrainSize = terrain->Vertices.size()* sizeof(terrain->Vertices[0]);;
	CurrentMappedTerrain = component->entity;	
	IsTerrainSubmittedForEdit = true;
}

void Fracture::TerrainSystem::ReleaseTerrainFromEditing()
{	
	if (!IsTerrainSubmittedForEdit)
		return;

	EditMode = TerrainEditModeOptions::Selection;
	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return;
	
	CurrentMappedTerrain = 0;
	mVerticesForEdit = nullptr;
	IsTerrainSubmittedForEdit = false;
}

glm::vec3 Fracture::TerrainSystem::GetTerrainIntersectionPoint(Fracture::CameraComponent& camera,bool& intersects,float mouse_x, float mouse_y)
{
	OPTICK_EVENT();
	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& transform = SceneManager::GetComponent<TransformComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if(!terrain)
		return glm::vec3(0.0f);
	//const auto& terrainVerts = std::vector<Vertex>(current_terrian_data, current_terrian_data + terrain->Vertices.size());
	
	CameraSystem system;
	glm::vec3 rayOrigin = camera.Position; // Define your camera position
	glm::vec3 rayDirection = system.ScreenToWorldRay(camera, mouse_x, mouse_y,1920,1080);
	glm::vec3 intersection;
	
	// Calculate the terrain boundaries
	float terrainWidth = component->TerrianSizeX;
	float terrainHeight = component->TerrianSizeY;
	
	// Traverse the heightmap along the ray
	glm::vec3 step = glm::normalize(rayDirection) * 1.0f;  // Assume square terrain cells
	glm::vec3 currentPos = rayOrigin;

	
	for (int i = 0; i < 1000; ++i) {  // Arbitrary large number to prevent infinite loop	
		// Calculate local position relative to terrain center
		glm::vec3 localPos = currentPos;// - transform->Position
		// Convert local position to heightmap grid coordinates
		float normalizedX = (localPos.x + terrainWidth / 2) / terrainWidth * component->TerrianSizeX;
		float normalizedZ = (localPos.z + terrainHeight / 2) / terrainHeight * component->TerrianSizeY;
	
		int gridX = static_cast<int>(normalizedX);
		int gridZ = static_cast<int>(normalizedZ);

	
		if (gridX < 0 || gridX >= component->TerrianSizeX - 1 || gridZ < 0 || gridZ >= component->TerrianSizeY - 1) {
			currentPos += step;
			continue;  // Ensure we don't go out of bounds
		}

		// Bilinear interpolation to find height at current position
		float h00 = terrain->HeightMapData[gridZ * terrainWidth + gridX];
		float h01 = terrain->HeightMapData[gridZ * terrainWidth + (gridX + 1)];
		float h10 = terrain->HeightMapData[(gridZ + 1) * terrainWidth + gridX];
		float h11 = terrain->HeightMapData[(gridZ + 1) * terrainWidth + (gridX + 1)];

		float tx = normalizedX - gridX;
		float tz = normalizedZ - gridZ;


		float height = (1 - tx) * ((1 - tz) * h00 + tz * h10) + tx * ((1 - tz) * h01 + tz * h11);

		if (currentPos.y <= height) {		
			intersects = true;
			//mHitpoints.push_back(glm::vec3(currentPos.x, height, currentPos.z));
			return glm::vec3(currentPos.x, height, currentPos.z);
		}

		currentPos += step;		
	}
	


	intersects = false;
	return glm::vec3(-1.0f);
}

bool Fracture::TerrainSystem::IsEditing()
{
	return IsTerrainSubmittedForEdit;
}

float Fracture::TerrainSystem::GetHeightatPoint(int index)
{
	

	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return 0.0f;

	if (index < 0 || index >= terrain->Vertices.size())
		return 0.0f;

	return terrain->Vertices[index].Position.y;
}

glm::vec3 Fracture::TerrainSystem::CalcNormal(int index)
{
	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return glm::vec3(0);

	glm::vec2 off = glm::vec2(1.0, 1.0);
	float hL = GetHeightatPoint(index - 1);
	float hR = GetHeightatPoint(index + 1);
	float hD = GetHeightatPoint(index - 1);
	float hU = GetHeightatPoint(index + 1);
	// deduce terrain normal
	glm::vec3 N;
	N.x = hL - hR;
	N.y = hD - hU;
	N.z = 2.0;
	N = normalize(N);
	return N;
}

float Fracture::TerrainSystem::RandomRange(float Min, float Max)
{
	// Create a random device to seed the generator
	static std::random_device rd;
	// Use the Mersenne Twister engine
	static std::mt19937 gen(rd());
	// Define a uniform real distribution in the range [Min, Max]
	std::uniform_real_distribution<float> dis(Min, Max);

	// Generate and return the random number
	return dis(gen);
}

float Fracture::TerrainSystem::sqrMagnitude(const glm::vec3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

