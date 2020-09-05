#pragma once
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component/Component.h"
#include <memory>
#include <string>

namespace Fracture
{
	class Model;
	class RenderComponent :public Component
	{
	public:
		RenderComponent(int entityID, std::string model);
		~RenderComponent();

		virtual void onAttach();
		virtual void onDettach();

		std::shared_ptr<Model> GetModel();

	private:
		std::string m_modelName;
		std::shared_ptr<Model> m_model;
	};

}

#endif