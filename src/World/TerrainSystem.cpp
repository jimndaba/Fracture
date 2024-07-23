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

void Fracture::TerrainSystem::GenerateTerrain(Fracture::UUID entity)
{
	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);	
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return;

	{
		if (component->HasHeightMap)
		{
			int width, height, nChannels;
			unsigned char* data = stbi_load(component->HeightMapPath.c_str(),
				&width, &height, &nChannels,
				0);
			
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
			component->TerrianSizeX = width;
			component->TerrianSizeY = height;
			desc.Width = (int)component->TerrianSizeX;
			desc.Height = (int)component->TerrianSizeY;			
			terrain->HeightMapData.resize(component->TerrianSizeY * component->TerrianSizeX, 0.0f);
			desc.texture_data = data;

			for (unsigned int i = 0; i < component->TerrianSizeY; i++)
			{
				for (unsigned int j = 0; j < component->TerrianSizeX; j++)
				{				
					unsigned char* texel = data + (j + width * i) * nChannels;
					// raw height at coordinate
					unsigned char y = texel[0];
					terrain->HeightMapData[i * width + j] = (float)y;
				}
			}
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
			desc.Name = "HeightMap";
			desc.Width = (int)component->TerrianSizeX;
			desc.Height = (int)component->TerrianSizeY;
			desc.texture_data = terrain->HeightMapData.data();
			AssetManager::Instance()->AddTexture(desc);
		}
		{
			TextureCreationInfo mixmap_desc;
			mixmap_desc.ID = UUID();
			mixmap_desc.TextureTarget = TextureTarget::Texture2D;
			mixmap_desc.AttachmentTrgt = AttachmentTarget::Color;
			mixmap_desc.format = TextureFormat::RGB;
			mixmap_desc.formatType = TextureFormatType::Float;
			mixmap_desc.internalFormat = InternalFormat::RGB32F;
			mixmap_desc.minFilter = TextureMinFilter::Linear;
			mixmap_desc.magFilter = TextureMagFilter::Linear;
			mixmap_desc.Wrap = TextureWrap::ClampToEdge;
			mixmap_desc.Width = (int)component->TerrianSizeY;
			mixmap_desc.Height = (int)component->TerrianSizeX;
			mixmap_desc.Name = "MixMap";
			mixmap_desc.f_data.resize(component->TerrianSizeX * component->TerrianSizeY * 3, 1.0f);
			component->MixMapID = mixmap_desc.ID;
			component->HasMixMap = true;
			AssetManager::Instance()->AddTexture(mixmap_desc);
		}
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
			diff_desc.Name = "DiffuseAtlasMap";
			diff_desc.data.resize(diff_desc.Width * diff_desc.Height * diff_desc.NoChannels, 0);
			component->DiffuseTextureAtlasID =diff_desc.ID;
			component->HasDiffuseTextureAtlas = true;
			AssetManager::Instance()->AddTexture(diff_desc);
		}
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
	if (IsTerrainSubmittedForEdit)
	{
		const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
		const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

		if (component->HasHeightMap && terrain->IsDirty)
		{
			GraphicsDevice::Instance()->UpdateTexture<float>(component->HeightMapID, terrain->HeightMapData, 0, 0, component->TerrianSizeX, component->TerrianSizeY);
			terrain->IsDirty = false;
		}
		if (component->HasMixMap && component->IsMixMapDirty)
		{
			GraphicsDevice::Instance()->UpdateFloatTexture(component->MixMapID);
			component->IsMixMapDirty = false;
		}

		if (component->IsAtlasDirty)
		{
			UpdateTerrainAtlas();
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
				position.y = terrain->HeightMapData[(int)(int)mPlacementPoints[layer->PlacemenMapID][i].z * component->TerrianSizeX + (int)mPlacementPoints[layer->PlacemenMapID][i].x] * component->TerrianMaxHeight - component->TerrianYShift;
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
								terrain->HeightMapData[index] = std::clamp(terrain->HeightMapData[index] + normalizedInfluence, -1.0f, 1.0f);
								break;
							case TerrainBrush::BrushTypeOptions::LOWER:
								terrain->HeightMapData[index] = std::clamp(terrain->HeightMapData[index] - normalizedInfluence, -1.0f, 1.0f);
								break;
							case TerrainBrush::BrushTypeOptions::FLATTEN:
								terrain->HeightMapData[index] = glm::mix(terrain->HeightMapData[index], params.centre.y, influence);
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

		const auto& mixmap = AssetManager::GetTextureByID(component->MixMapID);
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
					//float normalizedInfluence = influence / 255.0f;
					float normalizedInfluence = 1.0f - (distance / MainBrush.radius) * (distance / MainBrush.radius);
					int index = y * component->TerrianSizeX * 3 + x * 3;
					int indexG = index + 1;
					int indexB = index + 2;

					if (index >= 0 && index + 1 < mixmap->Description.f_data.size()) {
						switch (MainBrush.BrushType) {
						case TerrainBrush::BrushTypeOptions::RAISE:
						{
							auto [id1, weight1] = DecodeIDAndWeight(mixmap->Description.f_data[index]); //From
							auto [id2, weight2] = DecodeIDAndWeight(mixmap->Description.f_data[indexG]);// To
							float mask = mixmap->Description.f_data[indexB];
							int id3 = params.TextureIndex; // New Texture ID being painted.
							float weight = component->TerrainTextures[params.TextureIndex].Weight;
							weight1 = component->TerrainTextures[id1].Weight;
							weight2 = component->TerrainTextures[id2].Weight;

							float newWeight = std::clamp(normalizedInfluence, 0.0f, 1.0f);
							
							if (weight == 0.0f) {
								id1 = id3;
								id2 = id1; // If weight is 0, set both IDs to id1
								weight1 = weight;
								weight2 = weight; // Reset the weight2 to weight1
							}
							if (id3 == id1)
							{
								weight1 = weight;// std::clamp(weight1 + newWeight, 0.0f, 1.0f);
								//weight2 = ;// std::clamp(weight2 - newWeight, 0.0f, 1.0f);
							}
							else if (id3 == id2 && id2 != id1)
							{
								//weight1 = std::clamp(weight1 - newWeight, 0.0f, 1.0f);
								weight2 = weight;// std::clamp(weight2 + newWeight, 0.0f, 1.0f);
							}
							else
							{
								id2 = id3;
								weight2 = weight;
							}

							if (weight2 > weight1)
							{
								std::swap(id1, id2);
								std::swap(weight1, weight2);
							}

							mask += newWeight;				

							mixmap->Description.f_data[index] = EncodeIDAndWeight(id1, weight1); //Id1 weights for comparison
							mixmap->Description.f_data[indexG] = EncodeIDAndWeight(id2, weight2); //Id2 weights for comparison
							mixmap->Description.f_data[indexB] = std::clamp(mask, 0.0f, 1.0f);// total weight mask for blending
							component->IsMixMapDirty = true;
							break;
						}
						case TerrainBrush::BrushTypeOptions::LOWER:
						{
							mixmap->Description.f_data[index] = std::clamp(mixmap->Description.f_data[index] - normalizedInfluence, 0.0f, 1.0f);
							mixmap->Description.f_data[index + 1] = 0;
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

void Fracture::TerrainSystem::UpdateTerrainAtlas()
{
	if (!IsTerrainSubmittedForEdit)
		return;

	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
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

void Fracture::TerrainSystem::SaveHeightMap(const std::string& path)
{
	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	GLsizei nrChannels = 1;
	GLsizei stride = nrChannels * component->TerrianSizeX;
	stride += (stride % 1) ? (1 - stride % 1) : 0;
	GLsizei bufferSize = stride * component->TerrianSizeY;
	std::vector<char> buffer(bufferSize);
	for (int i = 0; i < terrain->HeightMapData.size(); i++)
	{
		buffer[i] =(char)terrain->HeightMapData[i];
	}
	ImageLoader::SaveImage(path, component->TerrianSizeX, component->TerrianSizeY, nrChannels, buffer, stride);
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

