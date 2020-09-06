#pragma once
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component/Component.h"
#include <memory>
#include <string>

namespace Fracture
{
	class Model;
	class Material;
	class RenderComponent :public Component
	{
	public:
		RenderComponent(int entityID, std::string model,std::string material);
		~RenderComponent();

		virtual void onAttach();
		virtual void onDettach();

		std::shared_ptr<Model> model;
		std::shared_ptr<Material> material;

	private:
		std::string m_modelName;
		std::string m_materialName;
		

	};

}

#endif