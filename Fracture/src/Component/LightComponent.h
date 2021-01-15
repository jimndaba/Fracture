#pragma once
#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

#include "Component.h"
#include "ILight.h"
#include <memory>

namespace Fracture
{

	class LightComponent:public Component
	{

	public:
		LightComponent(uint32_t id,LightType lightType);
		~LightComponent();

		virtual void onStart();

		virtual void OnDebug();

		void SetAmbient(glm::vec4 amb);
		void SetDiffuse(glm::vec4 diff);
		void SetSpecular(glm::vec4 spec);
		void SetPosition(glm::vec3 pos);
		void SetDirection(glm::vec3 rot);	
		void SetConstant(float constant);
		void SetLinear(float linear);
		void SetQuadratic(float quad);
		void SetCutoff(float coff);
		void SetOuterCutOff(float outer);
		void SetIntensity(float value);
		void SetRadiance(glm::vec3 value);

		glm::vec4 GetAmbient();
		glm::vec4 GetDiffuse();
		glm::vec4 GetSpecular();
		glm::vec3 GetPosition();
		glm::vec3 GetDirection();	
		float GetConstant();
		float GetLinear();
		float GetQuadratic();
		float GetCutoff();
		float GetOuterCutOff();
		float Intensity();
		glm::vec3 GetRadiance();

		std::shared_ptr<ILight> GetLight();
		LightType GetLightType();
		void ChangeLightType(LightType new_type);
		void ChangeEnvironment(const std::string& name);
		bool CastShadow();
		void SetCastShadow(bool value);

		void Accept(ISceneProbe* visitor) override;

	private:
		LightType m_type;
		std::shared_ptr<ILight> m_light;
	};

}

#endif