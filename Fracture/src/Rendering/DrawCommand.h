#pragma once
#ifndef DRAWCOMMAND_H
#define DRAWCOMMAND_H

#include <memory>
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"

namespace Fracture
{

	class DrawCommand
	{
	public:
		DrawCommand()
		{
		}

		~DrawCommand()
		{

		}

		uint32_t ID;
		uint32_t VAO;
		float ViewDepth;
		int HasTransparency;
		bool IsOutlined;
		GLint indiceSize;
		Material* material;
		std::vector<std::string> TextureNames;

		bool operator < (const DrawCommand& other) const
		{
			if (HasTransparency < other.HasTransparency) return true;
			if (other.HasTransparency < HasTransparency) return false;

			//Nearer objects are drawn first, since they hide further objects
			//Recall that GL view depth is along the negative Z direction,
			//so nearer objects have a greater Z.
			//if (ViewDepth > other.ViewDepth) return true;
			//if (other.ViewDepth > ViewDepth) return false;

			if (material->getShader()->ID() < other.material->getShader()->ID())
				return true;
			if (other.material->getShader()->ID() < material->getShader()->ID())
				return false;

			if (ID < other.ID) return true;
			if (other.ID < ID) return false;

			return false;
		}

		bool operator > (const DrawCommand& other) const
		{
			if (HasTransparency < other.HasTransparency) return true;
			if (other.HasTransparency < HasTransparency) return false;

			//Nearer objects are drawn first, since they hide further objects
			//Recall that GL view depth is along the negative Z direction,
			//so nearer objects have a greater Z.
			//if (ViewDepth > other.ViewDepth) return true;
			//if (other.ViewDepth > ViewDepth) return false;

			if (material->getShader()->ID() < other.material->getShader()->ID())
				return true;
			if (other.material->getShader()->ID() < material->getShader()->ID())
				return false;

			if (ID < other.ID) return true;
			if (other.ID < ID) return false;

			return false;
		}

	};

}

#endif 