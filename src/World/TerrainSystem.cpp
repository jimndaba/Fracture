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

void Fracture::TerrainSystem::GenerateTerrain(Fracture::UUID entity)
{

	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);	
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return;

	// load height map texture
	terrain->HeightMap.resize(component->TerrianSizeY, std::vector<float>(component->TerrianSizeX, 0.0f));

	if (component->HasHeightMap)
	{
		const auto& heightmap = AssetManager::Instance()->GetTextureByID(component->HeightMapID); int width, height, nChannels;
		unsigned char* data = stbi_load(component->HeightMapPath.c_str(),
			&width, &height, &nChannels,
			0);

		for (unsigned int i = 0; i < component->TerrianSizeY; i++)
		{
			for (unsigned int j = 0; j < component->TerrianSizeX; j++)
			{
				// retrieve texel for (i,j) tex coord
			// data() + (j + component->TerrianSizeX * i) * heightmap->Description.NoChannels;
				unsigned char* texel = data + (j + width * i) * nChannels;
				// raw height at coordinate
				unsigned char y = texel[0];
				terrain->HeightMap[i][j] =(float)y;// heightmap->Description.data[(j + component->TerrianSizeX * i) * heightmap->Description.NoChannels];
				//FRACTURE_INFO("text {}", (float)heightmap->Description.data[i, j]);;
			}
		}
	}

	for (int i = 0; i < component->TerrianSizeY; i++)
	{
		for (int j = 0; j < component->TerrianSizeX; j++)
		{
			Vertex v;
			v.Position.x = -component->TerrianSizeY / 2.0f + j;
			v.Position.y = terrain->HeightMap[i][j];
			v.Position.z = -component->TerrianSizeX / 2.0f + i;

			v.Normal = glm::vec3(0, 1.0f, 0);
			
			v.Uvs.x = (float)i / (float)component->TerrianSizeY;
			v.Uvs.y = (float)j / (float)component->TerrianSizeX;

			terrain->Vertices.push_back(v);
		}
	}	
	
	for (int i = 0; i < component->TerrianSizeY - 1; i++) // Row
	{
		for (int j = 0; j < component->TerrianSizeX; j++) // Col
		{			

			for (int k = 0; k < 2; k++)
			{			
				int v1 = j + component->TerrianSizeX * (i + k);
				terrain->Indices.push_back(v1);
			}		
		}
	}
	/*
	for (unsigned int z = 0; z < component->TerrianSizeY - 1; ++z) {
		if (z > 0) {
			// Add a degenerate triangle (same vertex as last and first in this strip)
			terrain->Indices.push_back(z * component->TerrianSizeX);
		}
		for (unsigned int x = 0; x < component->TerrianSizeX; ++x) {
			// Vertex at the current row
			terrain->Indices.push_back(z * component->TerrianSizeX + x);
			// Vertex at the next row
			terrain->Indices.push_back((z + 1) * component->TerrianSizeX + x);
		}
		if (z < component->TerrianSizeY - 2) {
			// Add a degenerate triangle (same vertex as last in this strip and first in the next)
			terrain->Indices.push_back((z + 1) * component->TerrianSizeX + (component->TerrianSizeX - 1));
		}
	}
*/
	//Let A be the vertex we are interseted in, B and C two other vertices of the same triangle
	//the normal of the triangle is then = Vector3.cross((B - A).normalized, (C - A).normalized).

	
	//calculate Normals	
	/*
	for (int i = 0; i < terrain->Indices.size() - 2; i++)
	{

		int index0 = terrain->Indices[i];
		int index1 = terrain->Indices[i + 1];
		int index2 = terrain->Indices[i + 2];

		glm::vec3 v0 = terrain->Vertices[index0].Position;
		glm::vec3 v1 = terrain->Vertices[index1].Position;
		glm::vec3 v2 = terrain->Vertices[index2].Position;

		// Check for degenerate triangles (skip if indices are the same)
		if (index0 == index1 || index1 == index2 || index2 == index0) {
			continue;
		}
				
		glm::vec3 edge1 = v1 - v0;
		glm::vec3 edge2 = v2 - v0;

		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

		float normalLength = glm::length(normal);
		if (normalLength > 1e-6) {			
			terrain->Vertices[index0].Normal += normal;
			terrain->Vertices[index1].Normal += normal;
			terrain->Vertices[index2].Normal += normal;
		}
		else
		{
			FRACTURE_ERROR("Nan");
		}
	}

	for (auto& vertex : terrain->Vertices) {
			vertex.Normal = glm::normalize(vertex.Normal);
		}
	*/
	for (int i = 0; i < terrain->Indices.size() - 2; i++)
	{
		int index0 = terrain->Indices[i];
		glm::vec2 p = glm::vec2(terrain->Vertices[index0].Position.x, terrain->Vertices[index0].Position.z);
		terrain->Vertices[index0].Normal = CalcNormal(index0);
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
		BufferDescription desc;
		desc.data = terrain->Vertices.data();
		desc.bufferType = BufferType::ArrayBuffer;
		desc.size = sizeof(terrain->Vertices[0]) * terrain->Vertices.size();
		desc.usage = BufferUsage::Static;
		desc.Name = "Verticies";
		desc.IsPersistantlyMapped = true;
		desc.BufferAccessFlags = BufferAccess::ReadWrite;
		terrain->VBO_Buffer = std::make_shared<Buffer>();
		GraphicsDevice::Instance()->CreateBuffer(terrain->VBO_Buffer.get(), desc);
		GraphicsDevice::Instance()->VertexArray_BindVertexBuffer(terrain->VAO, 0, sizeof(terrain->Vertices[0]), terrain->VBO_Buffer->RenderID);
	}

	{
		BufferDescription desc;
		desc.data = terrain->Indices.data();
		desc.bufferType = BufferType::ElementArrayBuffer;
		desc.size = sizeof(terrain->Indices[0]) * terrain->Indices.size();
		desc.usage = BufferUsage::Static;
		desc.Name = "IndexBuffer";
		terrain->EBO_Buffer = std::make_shared<Buffer>();
		GraphicsDevice::Instance()->CreateBuffer(terrain->EBO_Buffer.get(), desc);
		GraphicsDevice::Instance()->VertexArray_BindIndexBuffers(terrain->VAO, terrain->EBO_Buffer->RenderID);
	}

	GraphicsDevice::Instance()->VertexArray_BindAttributes(terrain->VAO, info);
	component->IsGenerated = true;
}

void Fracture::TerrainSystem::UpdateTerrain(Fracture::UUID entity)
{
	OPTICK_EVENT();
	if (!current_terrian_data)
		return;

	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	const int numThreads = std::thread::hardware_concurrency();
	std::vector<std::thread> threads;
	std::mutex mtx;
	float brushRadiusSq = RaiseBrush.radius * RaiseBrush.radius;

	auto brushFunc = [&](int start, int end) {
		OPTICK_EVENT();
		for (int i = start; i < end; ++i)
		{					
			int index0 = terrain->Indices[i];
			glm::vec2 p = glm::vec2(terrain->Vertices[index0].Position.x, terrain->Vertices[index0].Position.z);	
			terrain->Vertices[index0].Normal = CalcNormal(index0);
		}
	};

	int chunkSize = terrain->Indices.size() / numThreads;
	for (int i = 0; i < numThreads; ++i) {
		int start = i * chunkSize;
		int end = (i == numThreads - 1) ? terrain->Indices.size() : start + chunkSize;
		threads.emplace_back(brushFunc, start, end);
	}

	for (auto& thread : threads) {
		thread.join();
	}
	GLsizeiptr bufferSize = terrain->Vertices.size() * sizeof(Vertex);
	memcpy(current_terrian_data, terrain->Vertices.data(), bufferSize);

}

void Fracture::TerrainSystem::OnInit()
{
}

void Fracture::TerrainSystem::OnUpdate()
{
	const auto& terrains = SceneManager::GetAllComponents<TerrainComponent>();
	for (const auto& comp : terrains)
	{
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
						if (map->IsDirty)
						{
							UpdateDensityMap(layer->PlacemenMapID);
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
				position.y = terrain->HeightMap[(int)(int)mPlacementPoints[layer->PlacemenMapID][i].z][(int)mPlacementPoints[layer->PlacemenMapID][i].x];
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
	if (!current_terrian_data)
		return;

	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);


	if (terrain->IsDirty)
	{
		UpdateTerrain(CurrentMappedTerrain);
		terrain->IsDirty = false;
	}
		
}

void Fracture::TerrainSystem::OnDebugDraw()
{
	if (!current_terrian_data)
		return;
		
}

void Fracture::TerrainSystem::ApplyBrush(float dt, glm::vec3 centre)
{
	OPTICK_EVENT();
	if (!current_terrian_data)
		return;


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

		const int numThreads = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;
		std::mutex mtx;
		float brushRadiusSq = RaiseBrush.radius * RaiseBrush.radius;

		// Calculate normalized coordinates
		float normalizedX = static_cast<float>(centre.x) / static_cast<float>(component->TerrianSizeX) + 0.5f;
		float normalizedY = static_cast<float>(centre.z) / static_cast<float>(component->TerrianSizeY) + 0.5f;

		int heightMapX = static_cast<int>(normalizedX * component->TerrianSizeX);
		int heightMapY = static_cast<int>(normalizedY * component->TerrianSizeY);

		int startX = std::max(0, static_cast<int>(heightMapX - RaiseBrush.radius));
		int endX = std::min(component->TerrianSizeX, static_cast<int>(heightMapX + RaiseBrush.radius));
		int startY = std::max(0, static_cast<int>(heightMapY - RaiseBrush.radius));
		int endY = std::min(component->TerrianSizeY, static_cast<int>(heightMapY + RaiseBrush.radius));

		for (int y = startY; y < endY; ++y) {
			for (int x = startX; x < endX; ++x) {
				// Calculate the distance from the brush center
				float distance = std::sqrt((x - heightMapX) * (x - heightMapX) + (y - heightMapY) * (y - heightMapY));
				// Apply the influence if within the brush radius
				if (distance < RaiseBrush.radius) {
					float influence = (1.0f - (distance / RaiseBrush.radius)) * RaiseBrush.strength;
					switch (RaiseBrush.BrushType) {
					case TerrainBrush::BrushTypeOptions::RAISE:
						terrain->HeightMap[y][x] += influence;
						break;
					case TerrainBrush::BrushTypeOptions::LOWER:
						terrain->HeightMap[y][x] -= influence;
						break;
					case TerrainBrush::BrushTypeOptions::FLATTEN:
						terrain->HeightMap[y][x] = centre.y;
						break;
					}					
				}
			}
		}

		auto brushFunc = [&](int start, int end) {
			OPTICK_EVENT();
			for (int i = start; i < end; ++i)
			{
				glm::vec3 offset = terrain->Vertices[i].Position - centre;
				float distanceSq = glm::dot(offset, offset);
				if (distanceSq < brushRadiusSq) {
					float distance = glm::length(terrain->Vertices[i].Position - centre);
					float influence = (1.0f - (distance / RaiseBrush.radius)) * RaiseBrush.strength;
					switch (RaiseBrush.BrushType) {
					case TerrainBrush::BrushTypeOptions::RAISE:
						terrain->Vertices[i].Position.y += influence;
						break;
					case TerrainBrush::BrushTypeOptions::LOWER:
						terrain->Vertices[i].Position.y -= influence;
						break;
					case TerrainBrush::BrushTypeOptions::FLATTEN:
						terrain->Vertices[i].Position.y = centre.y;
						break;
					}
				}
			}
		};

		int chunkSize = terrain->Vertices.size() / numThreads;
		for (int i = 0; i < numThreads; ++i) {
			int start = i * chunkSize;
			int end = (i == numThreads - 1) ? terrain->Vertices.size() : start + chunkSize;
			threads.emplace_back(brushFunc, start, end);
		}

		for (auto& thread : threads) {
			thread.join();
		}

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
			float normalizedX = static_cast<float>(centre.x) / static_cast<float>(layer->Width) + 0.5f;
			float normalizedY = static_cast<float>(centre.z) / static_cast<float>(layer->Height) + 0.5f;

			int heightMapX = static_cast<int>(normalizedX * layer->Width);
			int heightMapY = static_cast<int>(normalizedY * layer->Height);

			int startX = std::max(0, static_cast<int>(heightMapX - RaiseBrush.radius));
			int endX = std::min(component->TerrianSizeX, static_cast<int>(heightMapX + RaiseBrush.radius));

			int startY = std::max(0, static_cast<int>(heightMapY - RaiseBrush.radius));
			int endY = std::min(component->TerrianSizeY, static_cast<int>(heightMapY + RaiseBrush.radius));

			for (int y = startY; y < endY; ++y) {
				for (int x = startX; x < endX; ++x) {
					// Calculate the distance from the brush center
					float distance = std::sqrt((x - heightMapX) * (x - heightMapX) + (y - heightMapY) * (y - heightMapY));
					// Apply the influence if within the brush radius
					if (distance < RaiseBrush.radius) {
						float influence = (1.0f - (distance / RaiseBrush.radius)) * 1.0f;
						switch (RaiseBrush.BrushType) {
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

/*
void Fracture::TerrainSystem::UpdateDensityGrid(Fracture::UUID placementID)
{
	if (mPlacementMaps.find(placementID) == mPlacementMaps.end()) return;
	
	//const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);
	//auto terrain = AssetManager::GetTerrainByID(component->TerrainID);

	//mCurrentGridForEdit->GridCellPositions.resize((mPlacementMaps[placementID]->Height / mCurrentGridForEdit->FootPrint) * (mPlacementMaps[placementID]->Width / mCurrentGridForEdit->FootPrint));

	for (float i = 0; i < mCurrentGridForEdit->GridCellPositions.size(); i += mCurrentGridForEdit->FootPrint)
	{
		mCurrentGridForEdit->GridCellPositions[(int)i][(int)j].x = (-component->TerrianSizeX / 2.0f) + (component->TerrianSizeX * j / (mCurrentGridForEdit->GriseSizeNoPoints.x - 1));
		mCurrentGridForEdit->GridCellPositions[(int)i][(int)j].z = (-component->TerrianSizeY / 2.0f) + (component->TerrianSizeY * i / (mCurrentGridForEdit->GriseSizeNoPoints.y - 1));
		mCurrentGridForEdit->GridCellPositions[i][j] = AddJitterToVector(mCurrentGridForEdit->GridCellPositions[i][j], (0.8f * mCurrentGridForEdit->GriseSizePx.x));

		// Calculate normalized coordinates
		float normalizedX = static_cast<float>(mCurrentGridForEdit->GridCellPositions[i][j].x) / static_cast<float>(component->TerrianSizeX) + 0.5f;
		float normalizedY = static_cast<float>(mCurrentGridForEdit->GridCellPositions[i][j].z) / static_cast<float>(component->TerrianSizeY) + 0.5f;
		int heightMapX = static_cast<int>(normalizedX * component->TerrianSizeX);
		int heightMapY = static_cast<int>(normalizedY * component->TerrianSizeY);
		heightMapX = std::clamp(heightMapX, 0, component->TerrianSizeX - 1);
		heightMapY = std::clamp(heightMapY, 0, component->TerrianSizeY - 1);
		mCurrentGridForEdit->GridCellPositions[i][j].y = terrain->HeightMap[heightMapY][heightMapX];
	}
	mCurrentGridForEdit->IsDirty = false;
}
*/
void Fracture::TerrainSystem::UpdateDensityMap(Fracture::UUID placementID)
{
	if (mPlacementMaps.find(placementID) == mPlacementMaps.end())
		return;

	if (mDenistyMaps.find(placementID) == mDenistyMaps.end())
		return;

	if (mModifiedPlacementMaps.find(placementID) == mModifiedPlacementMaps.end())
		return;

	mDenistyMaps[placementID]->Data = mModifiedPlacementMaps[placementID];
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
	//mCurrentGridForEdit->GridCellPositions[(int)i][(int)j].x = (-component->TerrianSizeX / 2.0f) 
	//mCurrentGridForEdit->GridCellPositions[(int)i][(int)j].z = (-component->TerrianSizeY / 2.0f) + (component->TerrianSizeY * i / (mCurrentGridForEdit->GriseSizeNoPoints.y - 1));

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
	if (!current_terrian_data)
		return;

	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);
	const auto& terrainVerts = std::vector<Vertex>(current_terrian_data, current_terrian_data + terrain->Vertices.size());


	GLsizei nrChannels = 1;
	GLsizei stride = nrChannels * component->TerrianSizeX;
	stride += (stride % 1) ? (1 - stride % 1) : 0;
	GLsizei bufferSize = stride * component->TerrianSizeY;
	std::vector<char> buffer(bufferSize);
	for (int i = 0; i < terrainVerts.size(); i++)
	{
		buffer[i] =(char)terrainVerts[i].Position.y;
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

	current_terrian_data = static_cast<Vertex*>(glMapNamedBufferRange(terrain->VBO_Buffer->RenderID, 0, OriginalTerrainSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT));

	if (!current_terrian_data)
		FRACTURE_ERROR("Failed to map buffer");
}

void Fracture::TerrainSystem::ReleaseTerrainFromEditing()
{
	if (!current_terrian_data)
		return;

	EditMode = TerrainEditModeOptions::Selection;


	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return;
	
	RenderCommands::UnMapbuffer(terrain->VBO_Buffer->RenderID);
	current_terrian_data = nullptr;
	CurrentMappedTerrain = 0;
	mVerticesForEdit = nullptr;
}

glm::vec3 Fracture::TerrainSystem::GetTerrainIntersectionPoint(Fracture::CameraComponent& camera,bool& intersects,float mouse_x, float mouse_y, float width, float height)
{
	OPTICK_EVENT();
	if (!current_terrian_data)
		return glm::vec3(0.0f);

	
	const auto& component = SceneManager::GetComponent<TerrainComponent>(CurrentMappedTerrain);
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	//const auto& terrainVerts = std::vector<Vertex>(current_terrian_data, current_terrian_data + terrain->Vertices.size());
	
	CameraSystem system;
	glm::vec3 rayOrigin = camera.Position; // Define your camera position
	glm::vec3 rayDirection = system.ScreenToWorldRay(camera, mouse_x, mouse_y,width,height);
	glm::vec3 intersection;

	int index = 0;
	for (auto i = terrain->Indices.begin(); i < terrain->Indices.end() - 2; i++) {
		const glm::vec3& v0 = terrain->Vertices[terrain->Indices[index]].Position;
		const glm::vec3& v1 = terrain->Vertices[terrain->Indices[index + 1]].Position;
		const glm::vec3& v2 = terrain->Vertices[terrain->Indices[index + 2]].Position;
		index++;

		if (system.IntersectRayTriangle(rayOrigin, rayDirection, v0, v1, v2, intersection)) 
		{
			intersects = true;
			return intersection;
		}

	}
	intersects = false;
	return glm::vec3(-1.0f);
}

bool Fracture::TerrainSystem::IsEditing()
{
	if (current_terrian_data)
		return true;
	return false;
}

float Fracture::TerrainSystem::GetHeightatPoint(int index)
{
	if (!current_terrian_data)
		return 0.0f;

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
	if (!current_terrian_data)
		return glm::vec3(0);

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

