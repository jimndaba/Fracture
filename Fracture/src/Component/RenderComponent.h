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
		RenderComponent(UUID id,const std::shared_ptr<Model>& model);
		~RenderComponent() = default;

		RenderComponent(const RenderComponent& component, UUID id) :Component(id)
		{
			m_isOutlined = component.m_isOutlined;
			m_material = component.m_material;
			m_model = component.m_model;
			m_modelName = component.m_modelName;
		}

		virtual void onStart();
		
		void SetMaterial(const std::string& oldmaterial, const std::string& newmaterial);
		std::shared_ptr<Material> GetMaterial();
		
		void SetModel(const std::string& name);
		std::shared_ptr<Model> GetModel() const;

		bool IsOutlined() { return m_isOutlined; }
		void SetIsOutlined(bool value) { m_isOutlined = value; }

		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<RenderComponent> clone(UUID id) const
		{
			return std::shared_ptr<RenderComponent>(this->clone_impl(id));
		}

		glm::vec4 Color;

	private:

		virtual RenderComponent* clone_impl(UUID id) const override
		{
			return new RenderComponent(*this, id);
		}

		std::shared_ptr<Model> m_model;
		std::string m_modelName;
		bool m_isOutlined = false;
		std::shared_ptr<Material> m_material;
	};

}

#endif