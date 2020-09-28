#include "Material.h"
#include "Shader.h"
#include <utility>
#include <functional>

Fracture::Material::Material(std::string name, std::shared_ptr<Shader> shader):Name(name),m_shader(shader)
{
	m_Uniforms = new  std::unordered_map<std::string, UniformValue>();
	m_SamplerUniforms =new  std::unordered_map<std::string, UniformValueSampler>();
}

Fracture::Material::~Material()
{
}

std::shared_ptr <Fracture:: Shader > Fracture::Material::getShader()
{
    return m_shader;
}

void Fracture::Material::setBool(std::string name, bool value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_BOOL;
	uniform.Bool = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setInt(std::string name, int value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_INT;
	uniform.Bool = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setFloat(std::string name, float value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_FLOAT;
	uniform.Float = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec2(std::string name, const glm::vec2& value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC2;
	uniform.Vec2 = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec2(std::string name, float x, float y) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC2;
	uniform.Vec2 = glm::vec2(x,y);
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec3(std::string name, const glm::vec3& value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC3;
	uniform.Vec3 = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec3(std::string name, float x, float y, float z) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC3;
	uniform.Vec3 = glm::vec3(x, y,z);
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec4(std::string name, const glm::vec4& value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC4;
	uniform.Vec4 = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec4(std::string name, float x, float y, float z, float w) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC4;
	uniform.Vec4 = glm::vec4(x, y,z,w);
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setMat2(std::string name, const glm::mat2& mat) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_MAT2;
	uniform.Mat2 = mat;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setMat3(std::string name, const glm::mat3& mat) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_MAT3;
	uniform.Mat3 = mat;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setMat4(std::string name, const glm::mat4& mat) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_MAT4;
	uniform.Mat4 = mat;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::SetTexture(std::string name, std::shared_ptr<Texture> value, unsigned int unit)
{
	UniformValueSampler sample;
	sample.Type = SHADER_TYPE::SHADER_TYPE_SAMPLER2D;
	sample.texture = value.get();
	sample.Unit = unit;
	m_SamplerUniforms->emplace(name,sample);
}

void Fracture::Material::ChangeTexture(std::string name, std::shared_ptr<Texture> value, unsigned int unit)
{
	m_SamplerUniforms->erase(name);
	UniformValueSampler sample;
	sample.Type = SHADER_TYPE::SHADER_TYPE_SAMPLER2D;
	sample.texture = value.get();
	sample.Unit = unit;
	m_SamplerUniforms->emplace(name, sample);
}

std::unordered_map<std::string, Fracture::UniformValue>* Fracture::Material::GetUniforms()
{
    return m_Uniforms;
}

std::unordered_map<std::string, Fracture::UniformValueSampler>* Fracture::Material::GetSamplerUniforms()
{
    return m_SamplerUniforms;
}
