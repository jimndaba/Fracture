#include "SceneSerializer.h"
#include "Entity/EntityManager.h"
#include "AssetManager/AssetManager.h"
#include "Component/ComponentManager.h"
#include "Component/TagComponent.h"
#include "Component/TransformComponent.h"
#include "Component/CameraControllerComponent.h"
#include "Component/BoxColliderComponent.h"
#include "Component/RelationshipComponent.h"
#include "Component/RenderComponent.h"
#include "Component/RigidBodyComponent.h"
#include "Component/LightComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"
#include <Fracture.h>


Fracture::SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene) : m_scene(scene)
{
}

Fracture::SceneSerializer::~SceneSerializer()
{
}

void Fracture::SceneSerializer::Serialize(const std::string& filepath)
{
	m_OutputStream.open(filepath);
	json j;
	j["Scene"] = "Untitled";

	json serialised_entities = json::array_t();
	for (auto& entity : m_scene->Entities())
	{		
		serialised_entities.push_back(SerializeEntity(entity));
	}
	j["Entities"] = serialised_entities;

	m_OutputStream << j;
	m_OutputStream.close();
}

bool Fracture::SceneSerializer::DeSerialize(const std::string& filepath)
{	
	std::ifstream stream(filepath);
	json input;

	if (!stream.good())
	{
		FRACTURE_ERROR("Can't read file");
		return false;
	}	

	stream >> input;
	if (input.is_null())
	{
		FRACTURE_ERROR("File is either non-json file or corrupt;");
		return false;
	}

	FRACTURE_TRACE("Fracture DeSerializing Scene");
	if (!exists(input,"Scene"))
	{
		FRACTURE_ERROR("No Fracture Scene in file!");
		return false;
	}

	if (exists(input, "Entities"))
	{
		for (auto entity : input["Entities"])
		{
			std::cout << entity["Entity ID"];
			DeSerializeEntity(entity);
		}
	}	
	return true;
}

nlohmann::json Fracture::SceneSerializer::SerializeEntity(std::shared_ptr<Entity> entity)
{
	json j;
	if (ComponentManager::HasComponent<TagComponent>(entity->Id))
	{
		std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity->Id);		
		json c;

		//Ealy Exit we do not want to serialize the Root Entity.
		if (tag->Name == "Root")
		{
			return j;
		}

		c["Entity Name"] = tag->Name;
		j["Tag Component"] = c;
	}

	if (ComponentManager::HasComponent<RelationShipComponent>(entity->Id))
	{
		std::shared_ptr<RelationShipComponent> component = ComponentManager::GetComponent<RelationShipComponent>(entity->Id);
		json c;		
		c["Has Parent"] = component->hasParent;
		c["Parent ID"] = component->m_parent ;
		c["No. Children"] = component->noChildren ;
		if (component->hasChildren())
		{
			json serialised_children = json::array_t();
			for (auto& child : component->GetChildren())
			{				
				json b;
				b["Child ID"] = child;
				serialised_children.push_back(b);
			}
			c["Children"]=  serialised_children;
		}

		j["Relationship Component"] = c;
	}

	if (ComponentManager::HasComponent<TransformComponent>(entity->Id))
	{
		std::shared_ptr<TransformComponent> component = ComponentManager::GetComponent<TransformComponent>(entity->Id);
		json c;	
		
		c["Position"] = { component->Position().x,component->Position().y,component->Position().z } ;
		c[ "Scale"] = { component->Scale().x,component->Scale().y ,component->Scale().z } ;
		c["Rotation"] =  { component->Rotation().x,component->Rotation().y ,component->Rotation().z } ;		
		j["Transform Component"] = c;
	}

	if (ComponentManager::HasComponent<CameraControllerComponent>(entity->Id))
	{
		std::shared_ptr<CameraControllerComponent> component = ComponentManager::GetComponent<CameraControllerComponent>(entity->Id);
		json c;
		c["Position"] = { component->Position.x,component->Position.y,component->Position.z };
		c["FOV"] = component->foV ;
		c["Near Clip"] = component->nearClip ;
		c["Far Clip"] =  component->farClip ;
		c["Front"] = { component->Front.x, component->Front.y, component->Front.z };
		c["Up"] = { component->Up.x,component->Up.y,component->Up.z };
		c["Right"] = { component->Right.x,component->Right.y,component->Right.z };
		c["Target Position"] = { component->m_TargetPosition.x, component->m_TargetPosition.y, component->m_TargetPosition.z };
		j["Camera Component"] = c;
	}

	if (ComponentManager::HasComponent<RenderComponent>(entity->Id))
	{
		std::shared_ptr<RenderComponent> component = ComponentManager::GetComponent<RenderComponent>(entity->Id);
		json c;
		c["Material"] = component->material->Name;
		json serialised_unfiorms= json::array_t();
		json serialised_sampleunfiorms = json::array_t();

		auto* uniforms = component->material->GetUniforms();
		for (auto value = uniforms->begin(); value != uniforms->end(); ++value)
		{
			json b;
			switch (value->second.Type)
			{
			case SHADER_TYPE_BOOL:	
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = value->second.Bool;
				break;
			case SHADER_TYPE_INT:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = value->second.Int;
				break;
			case SHADER_TYPE_FLOAT:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = value->second.Float;
				break;
			case SHADER_TYPE_VEC2:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Vec2.x,value->second.Vec2.y };
				break;
			case SHADER_TYPE_VEC3:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Vec3.x,value->second.Vec3.y,value->second.Vec3.z };
				break;
			case SHADER_TYPE_VEC4:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Vec4.x,value->second.Vec4.y,value->second.Vec4.z,value->second.Vec4.w };
				break;
			case SHADER_TYPE_COLOR3:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Color3.x,value->second.Color3.y,value->second.Color3.z};
				break;
			case SHADER_TYPE_COLOR4:
				b["Type"] = value->second.Type;
				b["Name"] = value->first;
				b["value"] = { value->second.Color4.x,value->second.Color4.y,value->second.Color4.z,value->second.Color4.w };
				break;
			case SHADER_TYPE_MAT2:
				
				break;
			case SHADER_TYPE_MAT3:
			
				break;
			case SHADER_TYPE_MAT4:
				
				break;
			default:
				//Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
				break;
			}
			serialised_unfiorms.push_back(b);
		}

		std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>* uniformsSamplers = component->material->GetSamplerUniforms();
		for (auto value = uniformsSamplers->begin(); value != uniformsSamplers->end(); ++value)
		{
			json b;
			switch (value->second->Type)
			{
			case SHADER_TYPE_SAMPLER2D:
				b["Type"] = value->second->Type;
				b["Name"] = value->first;
				b["Texture"] = value->second->texture->Name;
				b["Unit"] = value->second->Unit;
				break;
				FRACTURE_ERROR("Unrecognized Uniform type set");
				break;
			}
			serialised_sampleunfiorms.push_back(b);
		}

		c["MaterialUniforms"] = serialised_unfiorms;
		c["MaterialSampleUniforms"] = serialised_sampleunfiorms;
		c["Shader"]= component->material->getShader()->Name ;
		c["Model"] =component->model->Name ;
		j["Render Component"] = c;
	}

	if (ComponentManager::HasComponent<RigidBodyComponent>(entity->Id))
	{
		std::shared_ptr<RigidBodyComponent> component = ComponentManager::GetComponent<RigidBodyComponent>(entity->Id);
		json c;		
		c["Mass"] =  component->Mass ;
		j["Rigidbody Component"] = c;
	}

	if (ComponentManager::HasComponent<BoxColliderComponent>(entity->Id))
	{
		std::shared_ptr<BoxColliderComponent> component = ComponentManager::GetComponent<BoxColliderComponent>(entity->Id);
		json c;		
		c["Dimensions"] = {component->X,component->Y,component->Z };
		j["Box Collider Component"] =c; 
	}

	if (ComponentManager::HasComponent<LightComponent>(entity->Id))
	{
		std::shared_ptr<LightComponent> component = ComponentManager::GetComponent<LightComponent>(entity->Id);
		json c;

		switch (component->GetLightType())
		{
			case LightType::Sun:
			{
				c["Direction"] = { component->GetLight()->GetDirection().x,component->GetLight()->GetDirection().y,component->GetLight()->GetDirection().z };
				c["Ambient"] = { component->GetLight()->GetAmbient().x,component->GetLight()->GetAmbient().y,component->GetLight()->GetAmbient().z,component->GetLight()->GetAmbient().w };
				c["Diffuse"] = { component->GetLight()->GetDiffuse().x,component->GetLight()->GetDiffuse().y,component->GetLight()->GetDiffuse().z,component->GetLight()->GetDiffuse().w };
				c["Specular"] = { component->GetLight()->GetSpecular().x,component->GetLight()->GetSpecular().y,component->GetLight()->GetSpecular().z,component->GetLight()->GetSpecular().w };
				break;
			}
			case LightType::Spot:
			{
				c["Position"] = { component->GetLight()->GetPosition().x,component->GetLight()->GetPosition().y,component->GetLight()->GetPosition().z };
				c["Direction"] = { component->GetLight()->GetDirection().x,component->GetLight()->GetDirection().y,component->GetLight()->GetDirection().z };
				c["Ambient"] = { component->GetLight()->GetAmbient().x,component->GetLight()->GetAmbient().y,component->GetLight()->GetAmbient().z,component->GetLight()->GetAmbient().w };
				c["Diffuse"] = { component->GetLight()->GetDiffuse().x,component->GetLight()->GetDiffuse().y,component->GetLight()->GetDiffuse().z,component->GetLight()->GetDiffuse().w };
				c["Specular"] = { component->GetLight()->GetSpecular().x,component->GetLight()->GetSpecular().y,component->GetLight()->GetSpecular().z,component->GetLight()->GetSpecular().w };

				c["Linear"] = component->GetLight()->GetLinear();
				c["Constant"] = component->GetLight()->GetConstant();
				c["Qaudratic"] = component->GetLight()->GetQuadratic();
				c["Cutoff"] = component->GetLight()->GetCutoff();
				c["OuterCutoff"] = component->GetLight()->GetOuterCutOff();

				break;
			}
			case LightType::Point:
			{
				c["Position"] = { component->GetLight()->GetPosition().x,component->GetLight()->GetPosition().y,component->GetLight()->GetPosition().z };
				c["Ambient"] = { component->GetLight()->GetAmbient().x,component->GetLight()->GetAmbient().y,component->GetLight()->GetAmbient().z,component->GetLight()->GetAmbient().w };
				c["Diffuse"] = { component->GetLight()->GetDiffuse().x,component->GetLight()->GetDiffuse().y,component->GetLight()->GetDiffuse().z,component->GetLight()->GetDiffuse().w };
				c["Specular"] = { component->GetLight()->GetSpecular().x,component->GetLight()->GetSpecular().y,component->GetLight()->GetSpecular().z,component->GetLight()->GetSpecular().w };

				c["Linear"] = component->GetLight()->GetLinear();
				c["Constant"] = component->GetLight()->GetConstant();
				c["Qaudratic"] = component->GetLight()->GetQuadratic();
				break;
			}
		}
		j["Light Type"] = component->GetLightType();
		j["Light Component"] = c;
	}

	j["Entity ID"] = entity->Id;
	return j;
}

void Fracture::SceneSerializer::DeSerializeEntity(nlohmann::json j)
{
	if (exists(j, "Tag Component"))
	{
		//Early Exit if it is Root Entity.
		auto tagComponent = j["Tag Component"];
		if (tagComponent["Entity Name"] == "Root")
		{
			return;
		}

		std::shared_ptr<Entity> entity = EntityManager::CreateEntity(j["Entity ID"]);		
		std::shared_ptr<TagComponent> component = std::make_shared<TagComponent>(entity->Id);		
		component->SetName(tagComponent["Entity Name"]);
		ComponentManager::AddComponent<TagComponent>(component);

		if (exists(j, "Relationship Component"))
		{
			auto relationshipComponent = j["Relationship Component"];
			std::shared_ptr<RelationShipComponent> component = std::make_shared<RelationShipComponent>(entity->Id);

			bool hasParent = relationshipComponent["Has Parent"];
			if (hasParent)
			{
				std::cout << "Setting parent to: "<< relationshipComponent["Parent ID"] << "for Entity : " << entity->Id<< std::endl;
				uint32_t id = relationshipComponent["Parent ID"];
				component->SetParent(id);
			}
			
			auto children = relationshipComponent["Children"];

			for (auto child : children)
			{
				component->AddChild(child["Child ID"].get<uint32_t>());
			}

			ComponentManager::AddComponent<RelationShipComponent>(component);
		}

		if (exists(j, "Camera Component"))
		{
			auto cameraComponent = j["Camera Component"];
			float fov = cameraComponent["FOV"];
			float Near = cameraComponent["Near Clip"];
			float Far = cameraComponent["Far Clip"];
			std::array<float, 3> pos = cameraComponent["Position"];
			std::array<float, 3> up = cameraComponent["Up"];
			std::array<float, 3> front = cameraComponent["Front"];
			std::array<float, 3> right = cameraComponent["Right"];
			std::array<float, 3> targetpos = cameraComponent["Target Position"];

			std::shared_ptr<CameraControllerComponent> component = std::make_shared<CameraControllerComponent>(entity->Id);
			component->foV = fov;
			component->farClip = Far;
			component->nearClip = Near;
			component->Position = glm::vec3(pos[0], pos[1], pos[2]);
			component->Up = glm::vec3(up[0], up[1], up[2]);
			component->Right = glm::vec3(right[0], right[1], right[2]);
			component->Front= glm::vec3(front[0], front[1], front[2]);
			component->m_TargetPosition = glm::vec3(targetpos[0], targetpos[1], targetpos[2]);
			ComponentManager::AddComponent<CameraControllerComponent>(component);
			m_scene->setCamera(entity);
		}
		
		if (exists(j, "Transform Component"))
		{
			auto transformComponent = j["Transform Component"];
			std::array<float, 3> pos = transformComponent["Position"];
			std::array<float, 3> sc = transformComponent["Scale"];
			std::array<float, 3>rot = transformComponent["Rotation"];

			std::shared_ptr<TransformComponent> component = std::make_shared<TransformComponent>(entity->Id);
			component->setPosition(glm::vec3(pos[0], pos[1], pos[2]));
			component->setScale(glm::vec3(sc[0], sc[1], sc[2]));
			component->setRotation(glm::vec3(rot[0], rot[1], rot[2]));
			ComponentManager::AddComponent<TransformComponent>(component);

		}

		if (exists(j, "Render Component"))
		{
			auto renderComponent = j["Render Component"];
			std::string model = renderComponent["Model"];
			std::string shader_name = renderComponent["Shader"];
			std::string material_name = renderComponent["Material"];

			std::shared_ptr<Material> material = std::make_shared<Material>(material_name, AssetManager::getShader(shader_name));

			auto uniforms = renderComponent["MaterialUniforms"];			
			for (auto uniform : uniforms)
			{
				SHADER_TYPE shaderType = (SHADER_TYPE)uniform["Type"];
				switch (shaderType)
				{
				case SHADER_TYPE_BOOL:
				{
					material->setBool(uniform["Name"], (bool)uniform["value"]);
					break;
				}
				case SHADER_TYPE_INT:
				{
					material->setInt(uniform["Name"], (int)uniform["value"]);
					break;
				}
				case SHADER_TYPE_FLOAT:
				{
					material->setFloat(uniform["Name"], (float)uniform["value"]);
					break;
				}					
				case SHADER_TYPE_VEC2:					
				{
					std::array<float, 2> vec2 = uniform["value"];
					glm::vec2 value = glm::vec2(vec2[0], vec2[1]);
					material->setVec2(uniform["Name"], value);
					break;
				}
				case SHADER_TYPE_VEC3:
				{
					std::array<float, 3> vec3 = uniform["value"];
					glm::vec3 value = glm::vec3(vec3[0], vec3[1], vec3[2]);
					material->setVec3(uniform["Name"], value);
					break;
				}					
				case SHADER_TYPE_VEC4:
				{
					std::array<float, 4> vec4 = uniform["value"];
					glm::vec4 value = glm::vec4(vec4[0], vec4[1], vec4[2], vec4[3]);
					material->setVec4(uniform["Name"], value);
					break;
				}		
				case SHADER_TYPE_COLOR3:
				{
					std::array<float, 3> color3 = uniform["value"];
					glm::vec3 value = glm::vec3(color3[0], color3[1], color3[2]);
					material->setColor3(uniform["Name"], value);
					break;
				}
				case SHADER_TYPE_COLOR4:
				{
					std::array<float, 4> color4 = uniform["value"];
					glm::vec4 value = glm::vec4(color4[0], color4[1], color4[2], color4[3]);
					material->setColor4(uniform["Name"], value);
					break;
				}
				case SHADER_TYPE_MAT2:

					break;
				case SHADER_TYPE_MAT3:

					break;
				case SHADER_TYPE_MAT4:

					break;
				default:
					//Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
					break;
				}
				
			}

			auto uniformSamples = renderComponent["MaterialSampleUniforms"];
			for (auto sample : uniformSamples)
			{
				SHADER_TYPE shaderType = (SHADER_TYPE)sample["Type"];
				switch(shaderType)
				{
				case SHADER_TYPE_SAMPLER2D:
				{
					material->SetTexture(sample["Name"],AssetManager::getTexture(sample["Texture"]), (int)sample["Unit"]);
					break;
				}
				FRACTURE_ERROR("Unrecognized Uniform type set");
				break;
				}
			}

			AssetManager::AddMaterial(material_name,material);

			std::shared_ptr<RenderComponent> component = std::make_shared<RenderComponent>(entity->Id,model, material_name);
			ComponentManager::AddComponent<RenderComponent>(component);

		}
		
		if (exists(j, "Box Collider Component"))
		{
			auto boxColliderComponent = j["Box Collider Component"];
			std::array<float, 3>dimensions = boxColliderComponent["Dimensions"];

			std::shared_ptr<BoxColliderComponent> component = std::make_shared<BoxColliderComponent>(entity->Id,
				dimensions[0],
				dimensions[1],
				dimensions[2]);
			ComponentManager::AddComponent<BoxColliderComponent>(component);
		}

		if (exists(j, "Rigidbody Component"))
		{
			auto rigidbodyComponent = j["Rigidbody Component"];
			int mass = rigidbodyComponent["Mass"];
			std::shared_ptr<RigidBodyComponent> component = std::make_shared<RigidBodyComponent>(entity->Id, mass);
			ComponentManager::AddComponent<RigidBodyComponent>(component);
		}
		
		if (exists(j, "Light Component"))
		{
			auto lightComponent = j["Light Component"];
			std::array<float, 4> diffuse = lightComponent["Diffuse"];
			std::array<float, 4> ambient = lightComponent["Ambient"];
			std::array<float, 4> specular = lightComponent["Specular"];

			LightType lType = (LightType)j["Light Type"];

			switch (lType)
			{
			case LightType::Sun:
			{											
				std::shared_ptr<LightComponent> component = std::make_shared<LightComponent>(entity->Id,lType);
				std::array<float, 3> direction = lightComponent["Direction"];
				component->SetDirection(glm::vec3(direction[0], direction[1], direction[2]));
				component->SetAmbient(glm::vec4(ambient[0], ambient[1], ambient[2], ambient[3]));
				component->SetDiffuse(glm::vec4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]));
				component->SetSpecular(glm::vec4(specular[0], specular[1], specular[2], specular[3]));				
				ComponentManager::AddComponent<LightComponent>(component);
				ComponentManager::AddComponent<EditorNode>(entity->Id);
				break;
			}
			case LightType::Spot:
			{
				std::array<float, 3> direction = lightComponent["Direction"];
				std::array<float, 3> position = lightComponent["Position"];
				std::shared_ptr<LightComponent> component = std::make_shared<LightComponent>(entity->Id, lType);
				component->SetAmbient(glm::vec4(ambient[0], ambient[1], ambient[2], ambient[3]));
				component->SetDiffuse(glm::vec4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]));
				component->SetSpecular(glm::vec4(specular[0], specular[1], specular[2], specular[3]));
				component->SetDirection(glm::vec3(direction[0], direction[1], direction[2]));
				component->SetPosition(glm::vec3(position[0], position[1], position[2]));
				component->SetLinear(lightComponent["Linear"]);
				component->SetConstant(lightComponent["Constant"]);
				component->SetQuadratic(lightComponent["Qaudratic"]);
				component->SetCutoff(lightComponent["Cutoff"]);
				component->SetOuterCutOff(lightComponent["OuterCutoff"]);			
				ComponentManager::AddComponent<LightComponent>(component);
				ComponentManager::AddComponent<EditorNode>(entity->Id);
				break;
			}
			case LightType::Point:
			{				
				std::array<float, 3> position = lightComponent["Position"];
				std::shared_ptr<LightComponent> component = std::make_shared<LightComponent>(entity->Id, lType);
				component->SetAmbient(glm::vec4(ambient[0], ambient[1], ambient[2], ambient[3]));
				component->SetDiffuse(glm::vec4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]));
				component->SetSpecular(glm::vec4(specular[0], specular[1], specular[2], specular[3]));				
				component->SetPosition(glm::vec3(position[0], position[1], position[2]));
				component->SetLinear(lightComponent["Linear"]);
				component->SetConstant(lightComponent["Constant"]);
				component->SetQuadratic(lightComponent["Qaudratic"]);			
				ComponentManager::AddComponent<LightComponent>(component);
				ComponentManager::AddComponent<EditorNode>(entity->Id);
				break;
			}
			}
		}
		

		m_scene->addEntity(entity);
	}
}
