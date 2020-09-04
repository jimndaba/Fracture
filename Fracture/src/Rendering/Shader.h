#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include "GLM/matrix.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Fracture {

	class Shader
	{
	public:
		Shader(std::string vertexPath, std::string fragmentPath);
		~Shader();

		void use();
		void unbind();

		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec2(const std::string& name, glm::vec2& value) const;
		void setVec3(const std::string& name, glm::vec3& value) const;
		void setVec4(const std::string& name, glm::vec4& value) const;
		void setMat2(const std::string& name, glm::mat2& value) const;
		void setMat3(const std::string& name, glm::mat3& value) const;
		void setMat4(const std::string& name, glm::mat4& value) const;

	private:
		unsigned int m_vertex;
		unsigned int m_fragment;
		unsigned int m_program;

		void checkCompileErrors(unsigned int shader, std::string type);
	};

}

#endif
