#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "Color.h"

namespace Fracture
{
	enum class UniformType
	{
		INT,
		BOOL,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		IVEC2,
		IVEC3,
		IVEC4,
		BVEC2,
		BVEC3,
		BVEC4,
		COLOR3,
		COLOR4,
		MAT2,
		MAT3,
		MAT4,
		SAMPLE2D,
		SAMPLECUBE,
		SAMPLER1DARRAY,
		SAMPLER2DARRAY,
		SAMPLERCUBEARRAY,
		GLOBALDEPTH,
		GLOBALGRAB,
		GLOBALNORMAL,
		GLOBALPOSITION,
		GLOBALDELTATIME,
		NONE,
		TotalTypes
	};

	std::string UniformToString(UniformType t);

	struct Uniform
	{
		union Data
		{
			bool BOOL;
			float FLOAT;
			int INT;
			glm::vec2 VEC2;
			glm::vec3 VEC3;
			glm::vec4 VEC4;
			glm::vec3 COLOR3;
			glm::vec4 COLOR4;
			glm::mat2 MAT2;
			glm::mat3 MAT3;
			glm::mat4 MAT4;			
			//std::string TextureName;
			//uint32_t TextureID;
		};

		Uniform() {};
		Uniform(Fracture::UniformType uType, const std::string& name, Data mdata)
			:type(uType), Name(name), data(mdata) {}

		Uniform(Fracture::UniformType uType, const std::string& name, const std::string& texturename)
			:type(uType), Name(name), TextureName(texturename) {}

		inline bool operator == (const Uniform& other) const {
			return Name == other.Name;
		}


		std::string Name = "";
		UniformType type = UniformType::NONE;
		uint32_t TextureUnit = 0;
		std::string TextureName = "";
		Fracture::UUID TextureID;
		Data data = {};
	};

	struct ShaderUniform
	{
		std::string Name;
		UniformType ShaderUniformType;
		uint32_t Unit = 0;
	};

	struct ShaderDescription
	{
		std::string Name;
		std::string VertexPath;
		std::string FragmentPath;
		std::string GeometryPath;
		std::string ComputePath;
		std::string TesselationControlPath;
		std::string TesselationEvaluationPath;
		std::vector<ShaderUniform> UniformDescription;
	};

	enum class ShaderType
	{
		Vertex = GL_VERTEX_SHADER,
		Geometry = GL_GEOMETRY_SHADER,
		Fragement = GL_FRAGMENT_SHADER,
		Compute = GL_COMPUTE_SHADER,
		TessalationEvaluation = GL_TESS_EVALUATION_SHADER,
		TessalationControl = GL_TESS_CONTROL_SHADER
	};

	struct Shader
	{
		Shader(const ShaderDescription& desc);
		ShaderDescription Description;
		uint32_t Handle;
		UUID ID;
		bool IsLoaded = false;

		void SetUniform(const std::string name, const int& value);
		void SetUniform(const std::string name, const float& value);
		void SetUniform(const std::string name, const bool& value);
		void SetUniform(const std::string name, const glm::vec2& value);
		void SetUniform(const std::string name, const glm::vec3& value);
		void SetUniform(const std::string name, const glm::vec4& value);
		void SetUniform(const std::string name, const glm::mat3& value);
		void SetUniform(const std::string name, const glm::mat4& value);
		void SetUniform(const std::string name, const Fracture::Colour& value);
		void SetTexture(const std::string& name, const uint32_t& RenderID, unsigned int unit);
		
		std::unordered_map<std::string, GLint> m_ShaderLocationCache;
		GLint getUniformLocation(const std::string& name);
	
	};

}

#endif