#pragma once
#ifndef ASSETREGS_H
#define ASSETREGS_H


#include "Common/UUID.h"
#include <string>
#include "Rendering/Texture.h"

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
		uint32_t MeshTriangleCount;
		uint32_t datablockOffset;
		uint32_t SubMeshDataSize;
		uint32_t IndexDataSize;
		uint32_t VertexDataSize;
		uint32_t MeshTriangleSize;
		uint32_t BoneCount;
		uint32_t BoneSize;
		uint32_t AnimationCount;
	};


	//Texture Types: 0 - Normal, 1 HDR
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
		UUID ShaderID;
		std::string Name;
		std::string Path;
	};

	struct TerrainRegistry
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
		//
		TextureCreationInfo::TextureType Texture_Type;
		TextureTarget TextureTarget = TextureTarget::Texture2D;	
		InternalFormat internalFormat = InternalFormat::RGB8;
		TextureFormat format = TextureFormat::RGB;
		TextureFormatType formatType = TextureFormatType::UByte;
		TextureMagFilter magFilter = TextureMagFilter::Linear;
		TextureMinFilter minFilter = TextureMinFilter::Linear;
		TextureWrap Wrap = TextureWrap::ClampToEdge;
		int Width = 1;
		int Height = 1;
		int NoChannels = 1;
		bool GenMinMaps = false;

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
		std::string MetaPath;
	};

	struct AudioRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
	};

	struct AnimationClipRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
	};

	struct AnimationGraphRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
	};

	struct ParticleFxRegistry
	{
		UUID ID;
		std::string Name;
		std::string Path;
	};

}

#endif