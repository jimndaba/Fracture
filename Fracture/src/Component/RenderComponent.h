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
	struct BoundingBox;

	enum class RenderType
	{
		Normal,
		InstancedElementsIndirect,
		InstancedArrayIndirect,
	};
	

	class RenderComponent :public Component
	{
	public:
		RenderComponent(uint32_t entityID, std::shared_ptr<Model> model);
		~RenderComponent();


		virtual void onStart();
		void SetMaterial(std::string name);
		void SetModel(std::string name);

		void SetRenderType(RenderType m_type);
		RenderType getRenderType();

		std::shared_ptr<Model> m_model;

		bool IsOutlined() { return m_isOutlined; }
		void SetIsOutlined(bool value) { m_isOutlined = value; }

	private:
		std::string m_modelName;
		RenderType m_Rendertype;
		bool m_isOutlined = false;
	};

}

#endif