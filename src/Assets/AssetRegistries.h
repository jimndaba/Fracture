#pragma once
#ifndef ASSETREGS_H
#define ASSETREGS_H


#include "Common/UUID.h"
#include <string>

namespace Fracture
{
	struct MeshRegistry
	{
		enum class MeshType
		{
			Static,
			Skinned
		};
		UUID ID;
		std::string Name;
		std::string Path;
		MeshType meshType;
	};

	struct TextureRegistry
	{
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



}

#endif