#include "Material.h"
#include "Shader.h"
#include <utility>
#include <functional>
#include "AssetManager/AssetManager.h"
#include "Logging/Logger.h"

Fracture::Material::Material(const std::string& name, const std::shared_ptr<Shader>& shader) :
	Name(name),
	m_shader(shader),
	m_CastShadows(false),
	m_IsOutlined(false),
	m_IsTransparent(false),
	m_Uniforms(std::make_shared<std::unordered_map<std::string, UniformValue>>()),
	m_SamplerUniforms(std::make_shared<std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>>())
{
}

std::shared_ptr <Fracture:: Shader > Fracture::Material::getShader()
{
    return m_shader;
}

void Fracture::Material::setShader(const std::string& shader)
{
	m_shader = AssetManager::getShader(shader);
}

void Fracture::Material::use()
{
	m_shader->use();
}

void Fracture::Material::setBool(const std::string& name, bool value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_BOOL;
	uniform.Bool = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setInt(const std::string& name, int value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_INT;
	uniform.Bool = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setFloat(const std::string& name, float value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_FLOAT;
	uniform.Float = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec2(const std::string& name, const glm::vec2& value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC2;
	uniform.Vec2 = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec2(const std::string& name, float x, float y) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC2;
	uniform.Vec2 = glm::vec2(x,y);
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec3(const std::string& name, const glm::vec3& value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC3;
	uniform.Vec3 = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec3(const std::string& name, float x, float y, float z) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC3;
	uniform.Vec3 = glm::vec3(x, y,z);
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec4(const std::string& name, const glm::vec4& value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC4;
	uniform.Vec4 = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setVec4(const std::string& name, float x, float y, float z, float w) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_VEC4;
	uniform.Vec4 = glm::vec4(x, y,z,w);
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setColor3(const std::string& name, const glm::vec3& value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_COLOR3;
	uniform.Vec3 = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setColor3(const std::string& name, float x, float y, float z) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_COLOR3;
	uniform.Vec3 = glm::vec3(x, y, z);
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setColor4(const std::string& name, const glm::vec4& value) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_COLOR4;
	uniform.Vec4 = value;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setColor4(const std::string& name, float x, float y, float z, float w) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_COLOR4;
	uniform.Vec4 = glm::vec4(x, y, z, w);
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setMat2(const std::string& name, const glm::mat2& mat) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_MAT2;
	uniform.Mat2 = mat;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setMat3(const std::string& name, const glm::mat3& mat) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_MAT3;
	uniform.Mat3 = mat;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::setMat4(const std::string& name, const glm::mat4& mat) const
{
	UniformValue uniform;
	uniform.Type = SHADER_TYPE::SHADER_TYPE_MAT4;
	uniform.Mat4 = mat;
	uniform.Name = name;
	m_Uniforms->emplace(name, uniform);
}

void Fracture::Material::SetTexture(const std::string& name, std::shared_ptr<Texture> value, unsigned int unit)
{
	std::shared_ptr<UniformValueSampler> sample = std::make_shared<UniformValueSampler>();
	sample->Type = SHADER_TYPE::SHADER_TYPE_SAMPLER2D;
	sample->texture = value.get();
	sample->Unit = unit;
	m_SamplerUniforms->emplace(name,sample);
}

void Fracture::Material::ChangeTexture(const std::string& name, std::shared_ptr<Texture> value, unsigned int unit)
{
	m_SamplerUniforms->erase(name);
	std::shared_ptr<UniformValueSampler> sample = std::make_shared<UniformValueSampler>();
	sample->Type = SHADER_TYPE::SHADER_TYPE_SAMPLER2D;
	sample->texture = value.get();
	sample->Unit = unit;
	m_SamplerUniforms->emplace(name,sample);
}

void Fracture::Material::setCubeMap(const std::string& name, const unsigned int value, unsigned int unit) const
{
	std::shared_ptr<UniformValueSampler> sample = std::make_shared<UniformValueSampler>();
	sample->Type = SHADER_TYPE::SHADER_TYPE_SAMPLERCUBE;
	sample->id = value;
	sample->Unit = unit;
	m_SamplerUniforms->emplace(name, sample);
}

void Fracture::Material::ClearSamples()
{
	m_SamplerUniforms->clear();
}

std::shared_ptr<std::unordered_map<std::string, Fracture::UniformValue>> Fracture::Material::GetUniforms()
{
    return m_Uniforms;
}

std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<Fracture::UniformValueSampler>>> Fracture::Material::GetSamplerUniforms()
{
    return m_SamplerUniforms;
}

void Fracture::Material::CopyUniforms(std::unordered_map<std::string, UniformValue>* copy)
{
	for (auto value = copy->begin(); value != copy->end(); ++value)
	{
		switch (value->second.Type)
		{
		case SHADER_TYPE_BOOL:
			setBool(value->first, value->second.Bool);
			break;
		case SHADER_TYPE_INT:
			setInt(value->first, value->second.Int);
			break;
		case SHADER_TYPE_FLOAT:
			setFloat(value->first, value->second.Float);
			break;
		case SHADER_TYPE_VEC2:
			setVec2(value->first, value->second.Vec2);
			break;
		case SHADER_TYPE_VEC3:
			setVec3(value->first, value->second.Vec3);
			break;
		case SHADER_TYPE_VEC4:
			setVec4(value->first, value->second.Vec4);
			break;
		case SHADER_TYPE_COLOR3:
			setColor3(value->first, value->second.Color3);
			break;
		case SHADER_TYPE_COLOR4:
			setColor4(value->first, value->second.Color4);
			break;
		case SHADER_TYPE_MAT2:
			setMat2(value->first, value->second.Mat2);
			break;
		case SHADER_TYPE_MAT3:
			setMat3(value->first, value->second.Mat3);
			break;
		case SHADER_TYPE_MAT4:
			setMat4(value->first, value->second.Mat4);
			break;
		default:
			FRACTURE_ERROR("Unrecognized Uniform type set.");
			break;
		}
	}
}

void Fracture::Material::CopySampleUniforms(std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>* copy)
{
	for (auto it = copy->begin(); it != copy->end(); ++it)
	{
		SetTexture(it->first,AssetManager::getTexture(it->second->texture->Name),it->second->Unit);
	}
}
