#pragma once
#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "AssetRegistries.h"

namespace Fracture
{
	struct StaticMesh;
	struct SkinnedMesh;

	
	class MeshLoader
	{
	public:
		MeshLoader();

		static MeshRegistry ImportStaticMesh(const std::string& path, const std::string& out_path = "Content");
		static std::shared_ptr<Fracture::StaticMesh> LoadStaticMesh(const std::string& path);



	private:
	
		static const uint32_t s_MeshImportFlags =
			aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
			aiProcess_Triangulate |             // Make sure we're triangles			
			aiProcess_GenNormals |              // Make sure we have legit normals
			aiProcess_GenUVCoords |            // Convert UVs if required 	
			aiProcess_RemoveRedundantMaterials |
			aiProcess_SortByPType |
			aiProcess_OptimizeMeshes |
			aiProcess_ValidateDataStructure |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_OptimizeGraph;

	};




}


#endif