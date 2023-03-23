#include "FracturePCH.h"
#include "MaterialSerialiser.h"
#include "Rendering/Material.h"
#include "Rendering/Shader.h"

Fracture::MaterialSerialiser::MaterialSerialiser(ISerialiser::IOMode mode, ISerialiser::SerialiseFormat format):ISerialiser(mode,format)
{

}

void Fracture::MaterialSerialiser::WriteMaterial(Fracture::Material* material)
{
	BeginStruct("Material");
	{
		Property("ID", material->ID);
		Property("Name", material->Name);
		Property("ShaderID", material->Shader);

		Property("HasAlbedo", material->HasAlbedoTexture);
		Property("HasEmission", material->HasEmissionTexture);
		Property("HasMetalness", material->HasMetalTexture);
		Property("HasNormal", material->HasNormalTexture);
		Property("HasRoughness", material->HasRoughnessTexture);
		Property("HasSpecular", material->HasSpecularTexture);

		Property("CastsShadows", material->CastsShadows);
		Property("IsTranslucent", material->IsTranslucent);


		BeginCollection("Uniforms");
		for (const auto& uniform : material->Uniforms)
		{
			writeUniforms(uniform);
		}
		EndCollection();
	}
	EndStruct();

}

std::shared_ptr<Fracture::Material> Fracture::MaterialSerialiser::ReadMaterial()
{
	if (BeginStruct("Material"))
	{
		auto material = std::make_shared<Material>();
		material->ID = ID("ID");
		material->Shader = ID("ShaderID");
		material->Name = STRING("Name");

		material->HasAlbedoTexture = BOOL("HasAlbedo");
		material->HasEmissionTexture = BOOL("HasEmission");
		material->HasMetalTexture = BOOL("HasMetal");
		material->HasNormalTexture = BOOL("HasNormal");
		material->HasRoughnessTexture = BOOL("HasRoughness");
		material->HasSpecularTexture = BOOL("HasSpecular");
		material->IsTranslucent = BOOL("IsTranslucent");
		material->CastsShadows = BOOL("CastsShadows");

		if (BeginCollection("Uniforms"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{				
				if (BeginStruct("Uniform"))
				{
					auto mType = (Fracture::UniformType)INT("Type");
					switch (mType)
					{

					case UniformType::INT:
					{
						material->SetUniform(STRING("Name"), INT("Value"));
						break;
					}
					case UniformType::BOOL:
					{
						material->SetUniform(STRING("Name"), BOOL("Value"));
						break;
					}
					case UniformType::FLOAT:
					{
						material->SetUniform(STRING("Name"), FLOAT("Value"));
						break;
					}
					case UniformType::VEC2:
					{
						material->SetUniform(STRING("Name"), VEC2("Value"));
						break;
					}
					case UniformType::VEC3:
					{
						material->SetUniform(STRING("Name"), VEC3("Value"));
						break;
					}
					case UniformType::VEC4:
					{
						material->SetUniform(STRING("Name"), VEC4("Value"));
						break;
					}
					case UniformType::COLOR3:
					{
						Fracture::Colour col(glm::vec4(VEC3("Value"), 1.0f));
						material->SetUniform(STRING("Name"), col);
						break;
					}
					case UniformType::COLOR4:
					{
						Fracture::Colour col(VEC4("Value"));
						material->SetUniform(STRING("Name"), col);
						break;
					}
					case UniformType::SAMPLE2D:
					{					
						material->SetTextureByID(STRING("Name"), UniformType::SAMPLE2D, ID("Value"));
						break;
					}
					case UniformType::SAMPLECUBE:
					{
						material->SetTextureByID(STRING("Name"), UniformType::SAMPLECUBE ,ID("Value"));
						break;
					}
					}
					EndStruct();
				}
				NextInCollection();
			}
			EndCollection();
		}

		EndStruct();

		return material;
	}
	FRACTURE_ERROR("Failed to load Material");
	return nullptr;
}

void Fracture::MaterialSerialiser::writeUniforms(Fracture::Uniform uniform)
{
	BeginStruct("Uniform");
	switch (uniform.type)
	{
		case UniformType::INT:
		{
			Property("Type",(int)uniform.type);
			Property("Name",uniform.Name);
			Property("Value",(int)uniform.data.INT);
			break;
		}
		case UniformType::BOOL:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.data.BOOL);
			break;
		}
		case UniformType::FLOAT:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.data.FLOAT);
			break;
		}
		case UniformType::VEC2:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.data.VEC2);
			break;
		}
		case UniformType::VEC3:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.data.VEC3);
			break;
		}
		case UniformType::VEC4:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.data.VEC3);
			break;
		}
		case UniformType::COLOR3:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.data.COLOR3);
			break;
		}
		case UniformType::COLOR4:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.data.COLOR4);
			break;
		}
		case UniformType::SAMPLE2D:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.TextureID);
			break;
		}
		case UniformType::SAMPLECUBE:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			Property("Value", uniform.TextureID);
			break;
		}
	}
	EndStruct();
}