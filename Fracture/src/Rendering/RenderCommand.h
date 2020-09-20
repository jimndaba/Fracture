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
			//mkey = new Key();
		}
		~RenderCommand()
		{
			//delete mkey;
		}
		
		//Key* mkey;

		unsigned int VAO;
		unsigned int VBO;
		unsigned int IAO;
		GLint indiceSize;

		std::shared_ptr<TransformComponent> transform;
		std::shared_ptr<Material> material;

		//std::shared_ptr<Mesh> mesh;
		
	};

}

#endif