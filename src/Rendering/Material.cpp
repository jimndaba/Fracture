#include "FracturePCH.h"
#include "Material.h"
#include "GraphicsDevice.h"


Fracture::Material::Material()
{

}

void Fracture::Material::SetUniform(const std::string name, const int& value)
{
	Uniform uniform;

	uniform.type = UniformType::INT;
	uniform.Name = name;
	uniform.data.INT = value;
	Uniforms.push_back(uniform);

}
void Fracture::Material::SetUniform(const std::string name, const float& value)
{
	Uniform uniform;
	uniform.type = UniformType::FLOAT;
	uniform.Name = name;
	uniform.data.FLOAT = value;
	Uniforms.push_back(uniform);
}
void Fracture::Material::SetUniform(const std::string name, const bool& value)
{
	Uniform uniform;
	uniform.type = UniformType::BOOL;
	uniform.Name = name;
	uniform.data.BOOL = value;
	Uniforms.push_back(uniform);

}
void Fracture::Material::SetUniform(const std::string name, const glm::vec2& value)
{
	Uniform uniform;
	uniform.type = UniformType::VEC2;
	uniform.Name = name;
	uniform.data.VEC2 = value;
	Uniforms.push_back(uniform);

}
void Fracture::Material::SetUniform(const std::string name, const glm::vec3& value)
{
	Uniform uniform;
	uniform.type = UniformType::VEC3;
	uniform.Name = name;
	uniform.data.VEC3 = value;
	Uniforms.push_back(uniform);

}
void Fracture::Material::SetUniform(const std::string name, const glm::vec4& value)
{
	Uniform uniform;
	uniform.type = UniformType::VEC4;
	uniform.Name = name;
	uniform.data.VEC4 = value;
	Uniforms.push_back(uniform);

}
void Fracture::Material::SetUniform(const std::string name, const Fracture::Colour& value)
{
	Uniform uniform;
	uniform.type = UniformType::COLOR4;
	uniform.Name = name;
	uniform.data.COLOR4 = glm::vec4(value.R, value.G, value.B, value.A);
	Uniforms.push_back(uniform);

}
void Fracture::Material::SetTexture(const std::string name, const UniformType& mtype, const std::string& texture_name)
{
	TextureUnits[name] = UnitCount++;

	Uniform uniform;
	uniform.type = mtype;
	uniform.Name = name;
	uniform.TextureName = texture_name;
	uniform.TextureUnit = TextureUnits[name];
	Uniforms.push_back(uniform);


}

void Fracture::Material::SetTextureByID(const std::string name, const UniformType& mtype, const Fracture::UUID& texture_id)
{
	TextureUnits[name] = UnitCount++;

	Uniform uniform;
	uniform.type = mtype;
	uniform.Name = name;
	uniform.TextureID = texture_id;
	uniform.TextureUnit = TextureUnits[name];
	Uniforms.push_back(uniform);

}