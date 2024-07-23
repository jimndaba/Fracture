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
		Property("HasAO", material->HasAOTexture);

		Property("AlbedoTexture", material->AlbedoTexture);
		Property("NormalTexture", material->NormalTexture);
		Property("SpecularTexture", material->SpecularTexture);
		Property("RoughnessTexture", material->RoughnessTexture);
		Property("MetallicTexture", material->MetallicTexture);
		Property("AOTexture", material->AOTexture);
		Property("EmissionTexture", material->EmmissionTexture);

		Property("AlbedoColour", material->AlbedoColour);
		Property("SpecularColour", material->SpecularColour);
		Property("SpecularIntensity", material->SpecularIntensity);
		Property("SpecularLevel", material->SpecularLevel);
		Property("RoughnessLevel", material->RoughnessLevel);
		Property("MetallicLevel", material->MetalicLevel);
		Property("EmissionColour", material->EmissionColour);
		Property("EmissionStrength", material->EmmisionStrength);
		Property("AOLevel", material->AOLevel);


		Property("CastsShadows", material->CastsShadows);
		Property("CullMode", (int)material->cullmode);
		Property("TextureSpace", (int)material->TextureSpace);
		Property("BlendMode", (int)material->BlendMode);
		Property("MaterialType", (int)material->MaterialType);
		Property("TextureTiling", material->TextureTiling);
		Property("IsTranslucent", material->IsTranslucent);
		Property("IsSkinned", material->IsSkinned);
		Property("IsAffectedWind", material->IsAffectedByWind);
		Property("IsInstanced", material->IsInstanced);
		Property("ShaderOverride", material->ShaderOverride);
		Property("DepthWrite", material->DepthWrite);


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
		material->HasMetalTexture = BOOL("HasMetalness");
		material->HasNormalTexture = BOOL("HasNormal");
		material->HasRoughnessTexture = BOOL("HasRoughness");
		material->HasSpecularTexture = BOOL("HasSpecular");
		material->HasAOTexture = BOOL("HasAO");
		material->IsTranslucent = BOOL("IsTranslucent");
		material->IsSkinned = BOOL("IsSkinned");
		material->IsInstanced = BOOL("IsInstanced");
		material->IsAffectedByWind = BOOL("IsAffectedWind");
		material->ShaderOverride = BOOL("ShaderOverride");
		material->CastsShadows = BOOL("CastsShadows");
		material->DepthWrite = BOOL("DepthWrite");

		material->AlbedoTexture = ID("AlbedoTexture");
		material->SpecularTexture = ID("SpecularTexture");
		material->NormalTexture = ID("NormalTexture");
		material->RoughnessTexture = ID("RoughnessTexture");
		material->MetallicTexture = ID("MetallicTexture");
		material->AOTexture = ID("AOTexture");
		material->EmmissionTexture = ID("EmissionTexture");

		material->AlbedoColour = VEC4("AlbedoColour");
		material->SpecularColour = VEC4("SpecularColour");
		material->EmissionColour = VEC4("EmissionColour");

		material->SpecularLevel = FLOAT("SpecularLevel");
		material->SpecularIntensity = FLOAT("SpecularIntensity");
		material->RoughnessLevel = FLOAT("RoughnessLevel");
		material->MetalicLevel = FLOAT("MetallicLevel");
		material->EmmisionStrength = FLOAT("EmissionStrength");
		material->AOLevel = FLOAT("AOLevel");
		
		
		material->cullmode = (CullMode)INT("CullMode");
		material->TextureSpace = (UVSpace)INT("TextureSpace");
		material->BlendMode = (BlendingModeOption)INT("BlendMode");
		material->MaterialType = (MaterialTypeOptions)INT("MaterialType");
		material->TextureTiling = FLOAT("TextureTiling");


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
					case UniformType::GLOBALDEPTH :
					{
						Uniform m;
						m.type = mType;
						m.Name = STRING("Name");
						material->Uniforms.push_back(m);
						break;
					}
					case UniformType::GLOBALGRAB:
					{
						Uniform m;
						m.type = mType;
						m.Name = STRING("Name");
						material->Uniforms.push_back(m);
						break;
					}
					case UniformType::GLOBALNORMAL:
					{
						Uniform m;
						m.type = mType;
						m.Name = STRING("Name");
						material->Uniforms.push_back(m);
						break;
					}
					case UniformType::GLOBALPOSITION :
					{
						Uniform m;
						m.type = mType;
						m.Name = STRING("Name");
						material->Uniforms.push_back(m);
						break;
					}
					case UniformType::GLOBALDELTATIME:
					{
						Uniform m;
						m.type = mType;
						m.Name = STRING("Name");
						material->Uniforms.push_back(m);
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
		case UniformType::GLOBALDEPTH:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			break;
		}
		case UniformType::GLOBALGRAB:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			break;
		}
		case UniformType::GLOBALNORMAL:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			break;
		}
		case UniformType::GLOBALPOSITION:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			break;
		}
		case UniformType::GLOBALDELTATIME:
		{
			Property("Type", (int)uniform.type);
			Property("Name", uniform.Name);
			break;
		}
	}
	EndStruct();
}