#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component/Component.h"
#include "ITransform.h"

namespace Fracture
{
	class TransformComponent:public Component,public ITransform
	{
	public:
		TransformComponent(UUID ID);
		TransformComponent(UUID ID, glm::vec3 pos);
		TransformComponent(UUID ID, glm::vec3 pos, glm::vec3 scale);
		TransformComponent(UUID ID, glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);
		~TransformComponent()  = default ;


		TransformComponent(const TransformComponent& transform):Component(transform)
		{
			m_LocalTransform = transform.m_LocalTransform;
			m_WorldTransform = transform.m_WorldTransform;
			m_Position = transform.m_Position;
			m_Scale = transform.m_Scale;
			m_Rotation = transform.m_Rotation;
		}

		TransformComponent(const TransformComponent& transform,const UUID& entityID) :Component(entityID)
		{
			m_LocalTransform = transform.m_LocalTransform;
			m_WorldTransform = transform.m_WorldTransform;
			m_Position = transform.m_Position;
			m_Scale = transform.m_Scale;
			m_Rotation = transform.m_Rotation;
		}

		const TransformComponent& operator = (const TransformComponent& transform)
		{
			m_LocalTransform = transform.m_LocalTransform;
			m_WorldTransform = transform.m_WorldTransform;
			m_Position = transform.m_Position;
			m_Scale = transform.m_Scale;
			m_Rotation = transform.m_Rotation;
			return *this;
		}

		virtual void onStart();

		void setPosition(const glm::vec3& value);
		void setScale(const glm::vec3& value);
		void setRotation(const glm::vec3& value);

		glm::vec3 Position() const;
		glm::vec3 Scale() const;
		glm::vec3 Rotation() const;

		glm::mat4 GetLocalTranform() override;
		glm::mat4 GetWorldTransform()override;

		void Accept(ISceneProbe* visitor)override;

		json serialise(const std::shared_ptr<ComponentSerialiser>& visitor) override;

		std::shared_ptr<TransformComponent> clone(UUID id) const
		{
			return std::shared_ptr<TransformComponent>(this->clone_impl(id));
		}

	private:
		
		virtual TransformComponent* clone_impl(UUID id) const override
		{
			return new TransformComponent(*this, id);
		}
		glm::mat4 m_LocalTransform = glm::mat4();
		glm::mat4 m_WorldTransform = glm::mat4();
		glm::vec3 m_Position = glm::vec3();
		glm::vec3 m_Scale = glm::vec3();
		glm::vec3 m_Rotation = glm::vec3();
	};

}

#endif
