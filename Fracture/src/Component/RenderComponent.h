#pragma once
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component/Component.h"
#include "Physics/CollisionFilter.h"
#include <memory>
#include <string>
#include "glm/glm.hpp"

namespace Fracture
{

	class Model;
	class Mesh;
	class Material;

	enum class RenderType
	{
		Normal,
		InstancedElementsIndirect,
		InstancedArrayIndirect,
	};

	struct BoundingBox
	{
		glm::vec3 min;
		glm::vec3 max;

		float Width() const
		{
			return max.x - min.x;
		}

		float Height() const
		{
			return max.y - min.y;
		}

		float Depth() const
		{
			return max.z - min.z;
		}


	};

	class RenderComponent :public Component
	{
	public:
		RenderComponent(uint32_t entityID, std::shared_ptr<Mesh> mesh,std::string material);
		~RenderComponent();


		virtual void onStart();
		void SetMaterial(std::string name);
		void SetModel(std::string name);

		void SetRenderType(RenderType m_type);
		RenderType getRenderType();

		std::shared_ptr<Mesh> m_mesh;
		std::shared_ptr<Material> material;

	private:
		std::string m_modelName;
		std::string m_materialName;
		RenderType m_Rendertype;
		BoundingBox m_bounds;
		

	};

}

#endif