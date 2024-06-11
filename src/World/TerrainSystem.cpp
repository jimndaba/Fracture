#include "FracturePCH.h"
#include "TerrainSystem.h"
#include "SceneManager.h"
#include "Assets/AssetManager.h"
#include "Rendering/Texture.h"
#include "Rendering/Terrain.h"
#include "Rendering/GraphicsDevice.h"

void Fracture::TerrainSystem::GenerateTerrain(Fracture::UUID entity)
{

	const auto& component = SceneManager::GetComponent<TerrainComponent>(entity);	
	const auto& terrain = AssetManager::GetTerrainByID(component->TerrainID);

	if (!terrain)
		return;

	terrain->HeightMap.resize(component->TerrianSizeX, std::vector<float>(component->TerrianSizeY, 0.0f));


	for (int i = 0; i < component->TerrianSizeY; i++)
	{
		for (int j = 0; j < component->TerrianSizeX; j++)
		{
			Vertex v;
			v.Position.x = -component->TerrianSizeY / 2.0f + i;
			v.Position.y = terrain->HeightMap[i][j] * component->TerrianYScale - component->TerrianYShift;
			v.Position.z = -component->TerrianSizeX / 2.0f + j;
			terrain->Vertices.push_back(v);
		}
	}
	
	for (unsigned int i = 0; i < component->TerrianSizeY - 1; i++)       // for each row a.k.a. each strip
	{
		for (unsigned int j = 0; j < component->TerrianSizeX; j++)      // for each column
		{
			for (unsigned int k = 0; k < 2; k++)      // for each side of the strip
			{
				terrain->Indices.push_back(j + component->TerrianSizeX * (i + k));
			}
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
		BufferDescription desc;
		desc.data = terrain->Vertices.data();
		desc.bufferType = BufferType::ArrayBuffer;
		desc.size = sizeof(terrain->Vertices[0]) * terrain->Vertices.size();
		desc.usage = BufferUsage::Dynamic;
		desc.Name = "Verticies";
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

void Fracture::TerrainSystem::UpdateTerrain(Fracture::UUID terrain)
{
}
