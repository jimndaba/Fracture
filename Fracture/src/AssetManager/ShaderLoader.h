#pragma once
#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <memory>
#include <string>

namespace Fracture
{
	class Shader;
	

	class ShaderLoader
	{
	public:
		ShaderLoader();

		static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& vertex, const std::string& fragment);

		static std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geometry);
	};
}

#endif