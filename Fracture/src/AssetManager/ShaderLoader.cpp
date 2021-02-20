#include "ShaderLoader.h"
#include "Rendering/Shader.h"

Fracture::ShaderLoader::ShaderLoader()
{
}

std::shared_ptr<Fracture::Shader> Fracture::ShaderLoader::LoadShader(const std::string& name, const std::string& vertex, const std::string& fragment)
{
	std::shared_ptr<Shader> m_shader = std::make_shared<Shader>(name, vertex, fragment);
	return m_shader;
}

std::shared_ptr<Fracture::Shader> Fracture::ShaderLoader::LoadShader(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geometry)
{
	std::shared_ptr<Shader> m_shader = std::make_shared<Shader>(name, vertex, fragment,geometry);
	return m_shader;
}
