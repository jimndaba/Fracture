#pragma once
#ifndef RENDERCOMMAND_H
#define RENDERCOMMAND_H

#include <memory>
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Material.h"
#include "Mesh.h"
#include "Key.h"

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
		
	

		unsigned int VAO;
		unsigned int texture_unit;
		unsigned int texture_id;
		GLint indiceSize;
		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<Material> material;


		
	};

}

#endif