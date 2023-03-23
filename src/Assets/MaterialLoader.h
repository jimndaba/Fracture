#pragma once
#ifndef MATERIALLOADER_H
#define MATERIALLOADER_H

namespace Fracture
{
	struct Material;

	struct MaterialLoader
	{
		MaterialLoader();


		static std::shared_ptr<Fracture::Material> LoadMaterial(const std::string& path);




	};



}

#endif