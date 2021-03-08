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


namespace Fracture
{
	class Shader;
    class Texture;
    class Texture2D;
    class TextuerMultiSample;
    class TextureCubeMap;

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
		Material(const std::string& name,const std::shared_ptr<Shader>& shader);
        ~Material() = default;


        Material(const Material& material2, const std::string& name)
        { 
            Name = name;
            m_CastShadows = material2.m_CastShadows;
            m_IsOutlined = material2.m_IsOutlined;
            m_IsTransparent = material2.m_IsTransparent;
            m_shader = material2.m_shader;
            m_SamplerUniforms = material2.m_SamplerUniforms;
            m_Uniforms = material2.m_Uniforms;
        }
        const Material& operator = (const Material& material2)
        {
            m_CastShadows = material2.m_CastShadows;
            m_IsOutlined = material2.m_IsOutlined;
            m_IsTransparent = material2.m_IsTransparent;
            m_shader = material2.m_shader;
            m_SamplerUniforms = material2.m_SamplerUniforms;
            m_Uniforms = material2.m_Uniforms;
            return *this;
        }

        std::shared_ptr<Material> Create(const std::string& name)
        {
            return std::shared_ptr<Material>(new Material(*this,name));
        }

		std::shared_ptr<Shader> getShader();
        void setShader(const std::string&);
        void use();
		std::string Name;
        int MaterialCount;

        void setBool(const std::string&, bool value) const;
        void setInt(const std::string&, int value) const;
        void setFloat(const std::string&, float value) const;
        void setVec2(const std::string&, const glm::vec2& value) const;
        void setVec2(const std::string&, float x, float y) const;
        void setVec3(const std::string&, const glm::vec3& value) const;
        void setVec3(const std::string&, float x, float y, float z) const;
        void setVec4(const std::string&, const glm::vec4& value) const;
        void setVec4(const std::string&, float x, float y, float z, float w) const;
        void setColor3(const std::string&, const glm::vec3& value) const;
        void setColor3(const std::string&, float x, float y, float z) const;
        void setColor4(const std::string&, const glm::vec4& value) const;
        void setColor4(const std::string&, float x, float y, float z, float w) const;
        void setMat2(const std::string&, const glm::mat2& mat) const;
        void setMat3(const std::string&, const glm::mat3& mat) const;
        void setMat4(const std::string&, const glm::mat4& mat) const;

        void SetTexture(const std::string&,const std::shared_ptr<Texture>& value, unsigned int unit);
        void ChangeTexture(const std::string&, std::shared_ptr<Texture2D> value, unsigned int unit);
        void setCubeMap(const std::string& name, const std::shared_ptr<TextureCubeMap>& value, unsigned int unit) const;

        void ClearSamples();

        std::shared_ptr<std::unordered_map<std::string, UniformValue>> GetUniforms();
        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>> GetSamplerUniforms();

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
        bool IsOutlined() const { return m_IsOutlined; }
        void SetIsOutlined(const bool& value) { m_IsOutlined = value; }

	private:
		std::shared_ptr<Shader> m_shader;
        std::shared_ptr<std::unordered_map<std::string, UniformValue>> m_Uniforms;
        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>> m_SamplerUniforms;
        bool m_CastShadows = true;
        bool m_IsTransparent = false;
        bool m_IsOutlined = false;
	};
}

#endif
