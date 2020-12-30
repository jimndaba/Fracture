#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#define GLM_FORCE_NO_CTOR_INIT 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "Texture.h"

namespace Fracture
{
	class Shader;

    enum SHADER_TYPE
    {
        SHADER_TYPE_BOOL,
        SHADER_TYPE_INT,
        SHADER_TYPE_FLOAT,
        SHADER_TYPE_SAMPLER1D,
        SHADER_TYPE_SAMPLER2D,
        SHADER_TYPE_SAMPLER3D,
        SHADER_TYPE_SAMPLERCUBE,
        SHADER_TYPE_VEC2,
        SHADER_TYPE_VEC3,
        SHADER_TYPE_VEC4,
        SHADER_TYPE_COLOR3,
        SHADER_TYPE_COLOR4,
        SHADER_TYPE_MAT2,
        SHADER_TYPE_MAT3,
        SHADER_TYPE_MAT4,
    };

    struct Uniform
    {
        SHADER_TYPE  Type;
        std::string  Name;
        int          Size;
        unsigned int Location;      
    };

    struct UniformValue
    {
        SHADER_TYPE Type;
        std::string Name;       
        union
        {
            bool       Bool;
            int        Int;
            float      Float;
            glm::vec2 Vec2;
            glm::vec3 Vec3;
            glm::vec4 Vec4;
            glm::vec3 Color3;
            glm::vec4 Color4;
            glm::mat2 Mat2;
            glm::mat3 Mat3;
            glm::mat4 Mat4;
        };

        UniformValue() {};
    };

    struct UniformValueSampler
    {
        SHADER_TYPE  Type;
        unsigned int Unit;
        unsigned int id;
        union
        {
            Texture* texture;
        };

        UniformValueSampler() :texture{}
        {}

        ~UniformValueSampler()
        {}
        
    };

	class Material
	{
	public:
		Material(std::string name, std::shared_ptr<Shader> shader);
		~Material();
		std::shared_ptr<Shader> getShader();
        void setShader(std::string shader);
        void use();
		std::string Name;

        void setBool(std::string name, bool value) const;
        void setInt(std::string name, int value) const;
        void setFloat(std::string name, float value) const;
        void setVec2(std::string name, const glm::vec2& value) const;
        void setVec2(std::string name, float x, float y) const;
        void setVec3(std::string name, const glm::vec3& value) const;
        void setVec3(std::string name, float x, float y, float z) const;
        void setVec4(std::string name, const glm::vec4& value) const;
        void setVec4(std::string name, float x, float y, float z, float w) const;
        void setColor3(std::string name, const glm::vec3& value) const;
        void setColor3(std::string name, float x, float y, float z) const;
        void setColor4(std::string name, const glm::vec4& value) const;
        void setColor4(std::string name, float x, float y, float z, float w) const;
        void setMat2(std::string name, const glm::mat2& mat) const;
        void setMat3(std::string name, const glm::mat3& mat) const;
        void setMat4(std::string name, const glm::mat4& mat) const;

        void SetTexture(std::string name, std::shared_ptr<Texture> value, unsigned int unit);
        void ChangeTexture(std::string name, std::shared_ptr<Texture> value, unsigned int unit);
        void setCubeMap(const std::string& name, const  unsigned int value, unsigned int unit) const;

        void ClearSamples();

        std::unordered_map<std::string, UniformValue>* GetUniforms();
        std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>*GetSamplerUniforms();

        void CopyUniforms(std::unordered_map<std::string, UniformValue>* copy);
        void CopySampleUniforms(std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>* copy );

        bool IsTransparent()
        {
            return m_IsTransparent;
        }
        void setIsTransparent(bool istransparent)
        {
            m_IsTransparent = istransparent;
        }
        bool CastShadows()
        {
            return m_CastShadows;
        }
        void setCastShadows(bool castshadows)
        {
            m_CastShadows = castshadows;
        }


	private:
		std::shared_ptr<Shader> m_shader;
        std::unordered_map<std::string, UniformValue>* m_Uniforms;
        std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>* m_SamplerUniforms;
        bool m_CastShadows = false;
        bool m_IsTransparent = false;
	};
}

#endif
