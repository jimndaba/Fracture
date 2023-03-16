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

		static std::shared_ptr<Fracture::StaticMesh> LoadStaticMesh(const std::string& path);



	private:


	};




}


#endif