#pragma once
#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component/Component.h"

namespace Fracture
{
	class Model;
	class Mesh;
	class Material;
	struct BoundingBox;

	class RenderComponent :public Component
	{
	public:
		RenderComponent(uint32_t entityID,const std::shared_ptr<Model>& model);
		~RenderComponent() = default;

		virtual void onStart();
		
		void SetMaterial(const std::string& oldmaterial, const std::string& newmaterial);
		std::shared_ptr<Material> GetMaterial();
		
		void SetModel(const std::string& name);
		std::shared_ptr<Model> GetModel() const;

		bool IsOutlined() { return m_isOutlined; }
		void SetIsOutlined(bool value) { m_isOutlined = value; }

		void Accept(ISceneProbe* visitor) override;

	private:
		std::shared_ptr<Model> m_model;
		std::string m_modelName;
		bool m_isOutlined = false;
		std::shared_ptr<Material> m_material;
	};

}

#endif