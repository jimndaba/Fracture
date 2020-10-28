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
	class Material;
	class RenderComponent :public Component
	{
	public:
		RenderComponent(uint32_t entityID, std::string model,std::string material);
		~RenderComponent();


		virtual void onStart();

		void SetMaterial(std::string name);
		void SetModel(std::string name);

		std::shared_ptr<Model> model;
		std::shared_ptr<Material> material;

		CollisionGroup collisionGroup;
		CollisionMask collisionMask;

	private:
		std::string m_modelName;
		std::string m_materialName;
		

	};

}

#endif