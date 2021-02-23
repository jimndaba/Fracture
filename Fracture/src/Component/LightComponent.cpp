#include "LightComponent.h"
#include "ComponentManager.h"
#include "TransformComponent.h"
#include "Entity/ILight.h"
#include "Entity/SkyLight.h"
#include "Entity/SunLight.h"
#include "Entity/PointLight.h"
#include "Entity/SpotLight.h"

Fracture::LightComponent::LightComponent(UUID id, LightType lightType):Component(id),m_type(lightType)
{
	switch (lightType)
	{
	case LightType::Sun:	
		m_light = std::make_shared<SunLight>();
		break;
	case LightType::Point:
		
		m_light = std::make_shared<PointLight>();
		break;
	case LightType::Spot:		
		m_light = std::make_shared<SpotLight>();
		break;
	case LightType::Sky:
		m_light = std::make_shared<SkyLight>();
		break;
	}
}

void Fracture::LightComponent::onStart()
{
}

void Fracture::LightComponent::SetAmbient(const glm::vec4& amb)
{
	m_light->SetAmbient(amb);
}

void Fracture::LightComponent::SetDiffuse(const glm::vec4& diff)
{
	m_light->SetDiffuse(diff);
}

void Fracture::LightComponent::SetSpecular(const glm::vec4& spec)
{
	m_light->SetSpecular(spec);
}

void Fracture::LightComponent::SetPosition(const glm::vec3& pos)
{
	m_light->SetPosition(pos);
}

void Fracture::LightComponent::SetDirection(const glm::vec3& rot)
{
	m_light->SetDirection(rot);
}

void Fracture::LightComponent::SetConstant(const float& constant)
{
	m_light->SetConstant(constant);
}

void Fracture::LightComponent::SetLinear(const float& linear)
{
	m_light->SetLinear(linear);
}

void Fracture::LightComponent::SetQuadratic(const float& quad)
{
	m_light->SetQuadratic(quad);
}

void Fracture::LightComponent::SetCutoff(const float& coff)
{
	m_light->SetCutoff(coff);
}

void Fracture::LightComponent::SetOuterCutOff(const float& outer)
{
	m_light->SetOuterCutOff(outer);
}

void Fracture::LightComponent::SetIntensity(const float& value)
{
	m_light->SetIntensity(value);
}

void Fracture::LightComponent::SetRadiance(const glm::vec3& value)
{
	std::shared_ptr<SunLight> sun = std::dynamic_pointer_cast<SunLight>(m_light);
	sun->SetRadiance(value);
}

glm::vec4 Fracture::LightComponent::GetAmbient()
{
	return m_light->GetAmbient();
}

glm::vec4 Fracture::LightComponent::GetDiffuse()
{
	return m_light->GetDiffuse();
}

glm::vec4 Fracture::LightComponent::GetSpecular()
{
	return m_light->GetSpecular();
}

glm::vec3 Fracture::LightComponent::GetPosition()
{
	return m_light->GetPosition();
}

glm::vec3 Fracture::LightComponent::GetDirection()
{
	return m_light->GetDirection();
}

float Fracture::LightComponent::GetConstant()
{
	return m_light->GetConstant();
}

float Fracture::LightComponent::GetLinear()
{
	return m_light->GetLinear();
}

float Fracture::LightComponent::GetQuadratic()
{
	return m_light->GetQuadratic();
}

float Fracture::LightComponent::GetCutoff()
{
	return m_light->GetCutoff();
}

float Fracture::LightComponent::GetOuterCutOff()
{
	return m_light->GetOuterCutOff();
}

float Fracture::LightComponent::Intensity()
{
	return m_light->Intensity();
}

glm::vec3 Fracture::LightComponent::GetRadiance()
{
	std::shared_ptr<SunLight> sun = std::dynamic_pointer_cast<SunLight>(m_light);
	return sun->GetRadiance();
}

std::shared_ptr<Fracture::ILight> Fracture::LightComponent::GetLight()
{
	return m_light;
}

Fracture::LightType Fracture::LightComponent::GetLightType()
{
	return m_type;
}

void Fracture::LightComponent::ChangeLightType(LightType new_type)
{
	if (m_light)
	{
		switch (new_type)
		{
		case LightType::Sun:
			m_type = new_type;
			m_light.reset();
			m_light = std::make_shared<SunLight>();
			
			break;
		case LightType::Point:
			m_type = new_type;
			m_light.reset();
			m_light = std::make_shared<PointLight>();			
			break;
		case LightType::Spot:
			m_type = new_type;
			m_light.reset();
			m_light = std::make_shared<SpotLight>();			
			break;
		}
	}	
}

void Fracture::LightComponent::ChangeEnvironment(const std::string& name)
{
	std::shared_ptr<SkyLight> sky = std::dynamic_pointer_cast<SkyLight>(m_light);
	sky->ChangeEnvironment(name);
}

bool Fracture::LightComponent::CastShadow()
{
	return m_light->CastShadows();
}

void Fracture::LightComponent::SetCastShadow(bool value)
{
	m_light->SetCastShadows(value);
}

void Fracture::LightComponent::Accept(ISceneProbe* visitor)
{
	visitor->VisitLightComponent(this);
}
