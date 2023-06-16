#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Color.h"
#include "PipelineState.h"

namespace Fracture
{
	enum class TEXTURESLOT
	{
		Albedo,
		Normal,
		Specular,
		Roughness,
		Metalness,
		Emmission,
		TotalSlots
	};

	enum class UVSpace
	{
		Object,
		Screen,
		World
	};

	struct Material
	{
		Material();

		inline bool operator == (const Material& other) const {
			return ID == other.ID;
		}

		void SetUniform(const std::string name, const int& value);
		void SetUniform(const std::string name, const float& value);
		void SetUniform(const std::string name, const bool& value);
		void SetUniform(const std::string name, const glm::vec2& value);
		void SetUniform(const std::string name, const glm::vec3& value);
		void SetUniform(const std::string name, const glm::vec4& value);
		void SetUniform(const std::string name, const Fracture::Colour& value);
		void SetTexture(const std::string name, const UniformType& mtype, const std::string& texture_name);
		void SetTextureByID(const std::string name, const UniformType& mtype, const Fracture::UUID& texture_name);

		//Material Flags
		bool HasAlbedoTexture = false;
		bool HasNormalTexture = false;
		bool HasSpecularTexture = false;
		bool HasRoughnessTexture = false;
		bool HasMetalTexture = false;
		bool HasEmissionTexture = false;

		CullMode cullmode = CullMode::Back;
		UVSpace TextureSpace = UVSpace::Object;
		float TextureTiling = 1.0f;

		bool CastsShadows;
		bool IsTranslucent;
		bool IsDirty;

		Fracture::UUID ID;
		Fracture::UUID Shader;
		std::string Name;
		std::vector<Uniform> Uniforms;
		std::map<std::string, uint32_t> TextureUnits;
		uint32_t UnitCount = 1;
	};

}

#endif