#pragma once
#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include <memory>
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Material.h"
#include "Shader.h"

namespace Fracture
{

	class RenderCommand
	{
	public:
		RenderCommand()
		{
		}

		~RenderCommand()
		{
		}		

		static void Clear(float r, float g, float b)
		{
		}
	};	
}

#endif