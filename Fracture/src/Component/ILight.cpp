#include "ILight.h"

void Fracture::ILight::SetIntensity(float value)
{
	m_intensity = value;
}

float Fracture::ILight::Intensity()
{
	return m_intensity;
}
