#pragma once
#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include <memory>
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Material.h"
#include "Mesh.h"

namespace Fracture
{

	class RenderCommand
	{
	public:
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<Material> material;
	};

}

#endif