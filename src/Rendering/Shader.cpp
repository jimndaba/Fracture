#include "FracturePCH.h"
#include "Shader.h"

Fracture::Shader::Shader(const ShaderDescription& desc) :
    Description(desc)
{
}

void Fracture::Shader::SetUniform(const std::string name, const int& value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Fracture::Shader::SetUniform(const std::string name, const float& value)
{
    glUniform1f(getUniformLocation(name), value);
}

void Fracture::Shader::SetUniform(const std::string name, const bool& value)
{
    glUniform1i(getUniformLocation(name), value);
}

void Fracture::Shader::SetUniform(const std::string name, const glm::vec2& value)
{
    glUniform2f(getUniformLocation(name), value.x, value.y);
}

void Fracture::Shader::SetUniform(const std::string name, const glm::vec3& value)
{
    glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
}

void Fracture::Shader::SetUniform(const std::string name, const glm::vec4& value)
{
    glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Fracture::Shader::SetUniform(const std::string name, const glm::mat3& value)
{
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Fracture::Shader::SetUniform(const std::string name, const glm::mat4& value)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Fracture::Shader::SetUniform(const std::string name, const Fracture::Colour& value)
{
    glUniform4f(getUniformLocation(name), value.R, value.G, value.B, value.A);
}

void Fracture::Shader::SetTexture(const std::string& name, const uint32_t& RenderID, unsigned int unit)
{
    glBindTextureUnit(unit, RenderID);
    glUniform1i(getUniformLocation(name), unit);
}

GLint Fracture::Shader::getUniformLocation(const std::string& name)
{
    if (m_ShaderLocationCache.find(name) != m_ShaderLocationCache.end())
        return m_ShaderLocationCache[name];

    GLint location = glGetUniformLocation(Handle, name.c_str());
    m_ShaderLocationCache[name] = location;
    return location;
}

std::string Fracture::UniformToString(UniformType t)
{
	const std::map<UniformType, std::string> MyEnumStrings {
		{ UniformType::INT, "INT"},
		{ UniformType::BOOL,"BOOL" },
		{ UniformType::FLOAT,"FLOAT" },
		{ UniformType::VEC2,"VEC2" },
		{ UniformType::VEC3,"VEC3" },
		{ UniformType::VEC4,"VEC4" },
		{ UniformType::IVEC2,"IVEC2" },
		{ UniformType::IVEC3,"IVEC3" },
		{ UniformType::IVEC4,"IVEC4" },
		{ UniformType::BVEC2,"BVEC2" },
		{ UniformType::BVEC3,"BVEC3" },
		{ UniformType::BVEC4,"BVEC4" },
		{ UniformType::COLOR3,"COLOR3" },
		{ UniformType::COLOR4,"COLOR4" },
		{ UniformType::MAT2,"MAT2" },
		{ UniformType::MAT3,"MAT3" },
		{ UniformType::MAT4,"MAT4" },
		{ UniformType::SAMPLE2D,"SAMPLE2D" },
		{ UniformType::SAMPLECUBE,"SAMPLECUBE" },
		{ UniformType::SAMPLER1DARRAY,"SAMPLER1DARRAY" },
		{ UniformType::SAMPLER2DARRAY,"SAMPLER2DARRAY" },
		{ UniformType::SAMPLERCUBEARRAY,"SAMPLERCUBEARRAT" },
		{ UniformType::GLOBALDEPTH,"GLOBALDEPTH" },
		{ UniformType::GLOBALGRAB,"GLOBALGRAB" },
		{ UniformType::GLOBALNORMAL, "GLOBALNORMAL" },
		{ UniformType::GLOBALPOSITION,"GLOBALPOSITION" },
		{ UniformType::GLOBALDELTATIME,"GLOBALDELTATIME" },
		{ UniformType::NONE,"NONE" }
	};
	auto   it = MyEnumStrings.find(t);
	return it == MyEnumStrings.end() ? "Out of range" : it->second;
}
