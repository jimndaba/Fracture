#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "Vertex.h"

namespace Fracture
{
	struct Buffer;
	struct Terrain 
	{
		Terrain(Fracture::UUID id);
		Fracture::UUID ID;
		Fracture::UUID HeightMapTexture;		
		std::vector<float> HeightMapData;

		std::vector<unsigned int> Indices;
		std::vector<Vertex> Vertices;

		uint32_t VAO;
		std::shared_ptr<Buffer> VBO_Buffer;
		std::shared_ptr<Buffer> EBO_Buffer;
		bool IsDirty;
	};



}



#endif