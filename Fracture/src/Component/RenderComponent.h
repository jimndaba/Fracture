#pragma once
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component/Component.h"
#include "Physics/CollisionFilter.h"
#include <memory>
#include <string>

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

	};

}

#endif