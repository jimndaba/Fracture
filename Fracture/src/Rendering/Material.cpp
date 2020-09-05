#include "Material.h"
#include "Shader.h"

Fracture::Material::Material(std::shared_ptr<Shader> shader):m_shader(shader)
{
}

Fracture::Material::~Material()
{
}

std::shared_ptr <Fracture:: Shader > Fracture::Material::getShader()
{
    return m_shader;
}
