#include "DirectLightComponent.h"

Fracture::DirectLightComponent::DirectLightComponent(int id):Component(id,ComponentType::DirectLight)
{
}

Fracture::DirectLightComponent::~DirectLightComponent()
{
}

void Fracture::DirectLightComponent::onAttach()
{
}

void Fracture::DirectLightComponent::onDettach()
{
}

void Fracture::DirectLightComponent::SetDirection(glm::vec3 direction)
{
	m_direction = direction;
}

void Fracture::DirectLightComponent::SetAmbient(glm::vec3 ambient)
{
	m_ambient = ambient;
}

void Fracture::DirectLightComponent::SetDiffuse(glm::vec3 diffuse)
{
	m_diffuse = diffuse;
}

void Fracture::DirectLightComponent::SetSpecular(glm::vec3 specular)
{
	m_specular = specular;
}
