#pragma once
#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Component.h"



namespace Fracture
{
	class ILight;
	enum class LightType;
	class Environment;

	class LightComponent:public Component
	{

	public:
		LightComponent(UUID id,LightType lightType);
		~LightComponent() = default;

		LightComponent(const LightComponent& component, UUID id) :Component(id)
		{
			m_light = component.m_light;
			m_type = component.m_type;
		}

		virtual void onStart();

		void SetAmbient(const glm::vec4& amb);
		void SetDiffuse(const glm::vec4& diff);
		void SetSpecular(const glm::vec4& spec);
		void SetPosition(const glm::vec3& pos);
		void SetDirection(const glm::vec3& rot);
		void SetConstant(const float& constant);
		void SetLinear(const float&  linear);
		void SetQuadratic(const float& quad);
		void SetCutoff(const float& coff);
		void SetOuterCutOff(const float& outer);
		void SetIntensity(const float& value);
		void SetRadiance(const glm::vec3& value);

		glm::vec4 GetAmbient() const;
		glm::vec4 GetDiffuse()const;
		glm::vec4 GetSpecular()const;
		glm::vec3 GetPosition()const;
		glm::vec3 GetDirection()const;
		float GetConstant()const;
		float GetLinear()const;
		float GetQuadratic()const;
		float GetCutoff()const;
		float GetOuterCutOff()const;
		float Intensity()const;
		glm::vec3 GetRadiance()const;

		std::shared_ptr<ILight> GetLight();
		LightType GetLightType() const;
		void ChangeLightType(LightType new_type);
		std::shared_ptr<Environment> GetEnvironment()const;
		void ChangeEnvironment(const std::string& name);

		bool CastShadow();
		void SetCastShadow(bool value);

		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<LightComponent> clone(UUID id) const
		{
			return std::shared_ptr<LightComponent>(this->clone_impl(id));
		}

		json serialise(const std::shared_ptr<ComponentSerialiser>& visitor) override;

	private:

		virtual LightComponent* clone_impl(UUID id) const override
		{
			return new LightComponent(*this, id);
		}

		LightType m_type;
		std::shared_ptr<ILight> m_light;
	};

}

#endif