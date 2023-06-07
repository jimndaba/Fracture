#pragma once
#ifndef ASSETREGS_H
#define ASSETREGS_H


#include "Common/UUID.h"
#include <string>

namespace Fracture
{
	struct MeshRegistry
	{
		MeshRegistry(bool valid = true) :IsValid(valid) {};

		enum class MeshType
		{
			Static,
			Skinned
		};
		UUID ID;
		std::string Name;
		std::string Path;
		MeshType meshType;
		bool IsValid = false;
	};

	struct MeshFileHeader
	{		
		uint32_t magicValue;
		UUID ID;
		uint32_t SubMeshCount;
		uint32_t VertexCount;
		uint32_t IndexCount;
		uint32_t MaterialCount;
		uint32_t datablockOffset;
		uint32_t SubMeshDataSize;
		uint32_t IndexDataSize;
		uint32_t VertexDataSize;
	};

	struct TextureFileHeader
	{
		uint32_t magicValue;
		UUID ID;
		uint32_t Width;
		uint32_t Height;
		uint32_t Channels;
		uint32_t Count;
		uint32_t DataSize;
	};

	struct MaterialRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
	};

	struct TextureRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;	
		bool Success = false;
	};

	struct ShaderRegistry
	{
		UUID ID;
		std::string Name;
		std::string Vertex_Path;
		std::string Fragment_Path;
		std::string Geometry_Path;
		std::string Compute_Path;
		std::string TessalationControl_Path;
		std::string TessalationEval_Path;
	};

	struct SceneRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
	};

	struct LuaScriptRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
	};

	struct AudioRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
	};


}

#endif