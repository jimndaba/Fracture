#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>

namespace Fracture
{
	class Shader;

	class Material
	{
	public:
		Material(std::string name, std::shared_ptr<Shader> shader);
		~Material();
		std::shared_ptr<Shader> getShader();

		std::string Name;

	private:
		std::shared_ptr<Shader> m_shader;
	};
}

#endif
