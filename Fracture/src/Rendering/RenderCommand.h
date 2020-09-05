#pragma once
#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include <memory>
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Material.h"

namespace Fracture
{

	class RenderCommand
	{
	public:
		std::shared_ptr<RenderComponent> render;
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<Material> material;
	};

}

#endif