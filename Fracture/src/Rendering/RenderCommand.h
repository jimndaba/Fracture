#pragma once
#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include <memory>
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"

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

		uint32_t ID;
		uint32_t VAO;
		GLint indiceSize;
		Material* material;
		std::vector<std::shared_ptr<Texture>> Textures;
	};
}

#endif