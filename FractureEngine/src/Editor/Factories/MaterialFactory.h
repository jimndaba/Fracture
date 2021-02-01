#pragma once
#ifndef MATFACTORY_H
#define MATFACTORY_H

#include "Fracture.h"

namespace Fracture
{

	class MaterialFactory
	{


	public:
		MaterialFactory();
		~MaterialFactory();

		static std::shared_ptr<Material> OpaqueMaterial();
		static std::shared_ptr<Material> PBRMaterial(const std::string& name);
	};

}

#endif
