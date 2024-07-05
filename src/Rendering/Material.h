#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"
#include "Color.h"
#include "RenderCommands.h"

namespace Fracture
{
	struct RenderContext;

	enum class TEXTURESLOT
	{
		GlobalAO,
		DirectShadows,
		Albedo,
		Normal,
		Specular,
		Roughness,
		Metalness,
		Emmission,
		AO,
		Irradiance,
		Prefilter,
		BRDF,
		Reflections,
		GlobalGrab,
		GlobalDepth,
		GlobalPosition,
		GlobalNormal,
		GlobalSpots,
		GlobalPoint,
		TotalSlots
	};

	enum class UVSpace
	{
		Object,
		Screen,
		World
	};

	enum class MaterialShadingModel
	{
		BlinnPhong,
		PBR
	};

	enum class BlendingModeOption
	{
		Additive,
		Linear,
		Multiply
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
		bool HasAOTexture = false;
		bool HasEmissionTexture = false;

		Fracture::UUID AlbedoTexture;
		Fracture::UUID NormalTexture;
		Fracture::UUID SpecularTexture;
		Fracture::UUID RoughnessTexture;
		Fracture::UUID MetallicTexture;
		Fracture::UUID AOTexture;
		Fracture::UUID EmmissionTexture;


		std::function<void(Fracture::RenderContext* cntxt, Fracture::Shader* shader, int unit)> GlobalDepth;
		std::function<void(Fracture::RenderContext* cntxt, Fracture::Shader* shader, int unit)> GlobalGrab;
		std::function<void(Fracture::RenderContext* cntxt, Fracture::Shader* shader, int unit)> GlobalNormal;
		std::function<void(Fracture::RenderContext* cntxt, Fracture::Shader* shader, int unit)> GlobalPosition;
		std::function<void(Fracture::RenderContext* cntxt, Fracture::Shader* shader, float dt)> GlobalDeltaTime;


		glm::vec4 AlbedoColour = glm::vec4(1.0);
		glm::vec4 SpecularColour = glm::vec4(1.0);
		glm::vec4 EmissionColour = glm::vec4(0.0);;
		float RoughnessLevel = 0.5f;
		float SpecularLevel = 128.0f;
		float SpecularIntensity = 1.0f;
		float AOLevel = 1.0f;
		float MetalicLevel = 0.1f;
		float EmmisionStrength = 0.0f;




		CullMode cullmode = CullMode::Back;
		UVSpace TextureSpace = UVSpace::Object;
		BlendingModeOption BlendMode = BlendingModeOption::Linear;
		MaterialShadingModel ShadingModel = MaterialShadingModel::PBR;

		float TextureTiling = 1.0f;

		bool CastsShadows = true;
		bool IsTranslucent = false;
		bool IsReflective = false;
		bool DepthWrite = true;
		bool IsSkinned = false;
		bool IsInstanced = true;
		bool IsAffectedByWind = false;
		bool IsDirty = false;

		Fracture::UUID ID;
		Fracture::UUID Shader;
		std::string Name;
		std::vector<Uniform> Uniforms;
		std::map<std::string, uint32_t> TextureUnits;
		uint32_t UnitCount = 1;
	};

}

#endif