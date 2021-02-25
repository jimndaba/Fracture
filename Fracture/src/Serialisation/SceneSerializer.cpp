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
#include "Component/BillboardComponent.h"
#include "Component/AnimatorComponent.h"
#include "Entity/ILight.h"
#include "Entity/SkyLight.h"
#include "Rendering/Environment.h"
#include "Rendering/Material.h"
#include "Rendering/Model.h"
#include "Rendering/Shader.h"
#include "Scene/Scene.h"
#include "Entity/Entity.h"
#include "Rendering/OpenGL/Texture2D.h"
#include "Entity/UUID.h"

#include "ComponentSerialisers/ComponentSerialisers.h"

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
		json e;
		e["EntityID"] = (uint32_t)entity->GetId();
		//serialised_entities.push_back(SerializeEntity(entity));
		serialised_entities.push_back(e);
	}
	j["Entities"] = serialised_entities;
	
	auto& TagComponents = ComponentManager::GetAllComponents<TagComponent>();
	json serialised_tagc = json::array_t();
	for (auto& component : TagComponents)
	{
		serialised_tagc.push_back(component->serialise(std::make_shared<TagComponentSerialiser>()));
	}

	auto& TransformComponents = ComponentManager::GetAllComponents<TransformComponent>();
	json serialised_transformc = json::array_t();
	for (auto& component : TransformComponents)
	{
		serialised_transformc.push_back(component->serialise(std::make_shared<TransformComponentSerialiser>()));
	}

	auto& ScriptComponents = ComponentManager::GetAllComponents<ScriptComponent>();
	json serialised_Scriptc = json::array_t();
	for (auto& component : ScriptComponents)
	{
		serialised_Scriptc.push_back(component->serialise(std::make_shared<ScriptComponentSerialiser>()));
	}

	auto& RigidBodyComponents = ComponentManager::GetAllComponents<RigidBodyComponent>();
	json serialised_RigidBodyc = json::array_t();
	for (auto& component : RigidBodyComponents)
	{
		serialised_RigidBodyc.push_back(component->serialise(std::make_shared<RigidBodyComponentSerialiser>()));
	}

	auto& RenderComponents = ComponentManager::GetAllComponents<RenderComponent>();
	json serialised_Renderc = json::array_t();
	for (auto& component : RenderComponents)
	{
		serialised_Renderc.push_back(component->serialise(std::make_shared<RenderComponentSerialiser>()));
	}

	auto& RelationshipComponents = ComponentManager::GetAllComponents<RelationShipComponent>();
	json serialised_Relationc = json::array_t();
	for (auto& component : RelationshipComponents)
	{
		serialised_Relationc.push_back(component->serialise(std::make_shared<RelationShipComponentSerialiser>()));
	}

	auto& LightComponents = ComponentManager::GetAllComponents<LightComponent>();
	json serialised_Lightc = json::array_t();
	for (auto& component : LightComponents)
	{
		serialised_Lightc.push_back(component->serialise(std::make_shared<LightComponentSerialiser>()));
	}

	auto& CameraControllers = ComponentManager::GetAllComponents<CameraControllerComponent>();
	json serialised_Camerac = json::array_t();
	for (auto& component : CameraControllers)
	{
		serialised_Camerac.push_back(component->serialise(std::make_shared<CameraControllerComponentSerialiser>()));
	}

	auto& BoxColliderComponents = ComponentManager::GetAllComponents<BoxColliderComponent>();
	json serialised_Boxc = json::array_t();
	for (auto& component : BoxColliderComponents)
	{
		serialised_Boxc.push_back(component->serialise(std::make_shared<BoxColliderComponentSerialiser>()));
	}

	auto& BillboardComponents = ComponentManager::GetAllComponents<BillboardComponent>();
	json serialised_billboardc = json::array_t();
	for (auto& component : BillboardComponents)
	{
		serialised_billboardc.push_back(component->serialise(std::make_shared<BillboardComponentSerialiser>()));
	}

	auto& AnimationComponents = ComponentManager::GetAllComponents<AnimatorComponent>();
	json serialised_animatec = json::array_t();
	for (auto& component : AnimationComponents)
	{
		serialised_animatec.push_back(component->serialise(std::make_shared<AnimatorComponentSerialiser>()));
	}

	j["TagComponents"] = serialised_tagc;
	j["TransformComponents"] = serialised_transformc;
	j["AnimatorComponents"] = serialised_animatec;
	j["BillboardComponents"] = serialised_billboardc;
	j["BoxColliderComponents"] = serialised_Boxc;
	j["CameraComponents"] = serialised_Camerac;
	j["LightComponents"] = serialised_Lightc;
	j["RelationshipComponents"] = serialised_Relationc;
	j["RenderComponents"] = serialised_Renderc;
	j["RigidbodyComponents"] = serialised_RigidBodyc;

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

void Fracture::SceneSerializer::SerializeComponents(json j)
{
	
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

		UUID id = UUID(j["Entity ID"]);
		std::shared_ptr<Entity> entity = EntityManager::CreateEntity(id);		
		std::shared_ptr<TagComponent> component = std::make_shared<TagComponent>(entity->GetId());		
		component->SetName(tagComponent["Entity Name"]);
		ComponentManager::AddComponent<TagComponent>(component);

		if (exists(j, "Relationship Component"))
		{
			auto relationshipComponent = j["Relationship Component"];
			std::shared_ptr<RelationShipComponent> component = std::make_shared<RelationShipComponent>(entity->GetId());

			bool hasParent = relationshipComponent["Has Parent"];
			if (hasParent)
			{
				std::cout << "Setting parent to: "<< relationshipComponent["Parent ID"] << "for Entity : " << entity->GetId()<< std::endl;
				UUID p_id = UUID(relationshipComponent["Parent ID"]);
				component->SetParent(p_id);
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

			std::shared_ptr<CameraControllerComponent> component = std::make_shared<CameraControllerComponent>(entity->GetId());
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

			std::shared_ptr<TransformComponent> component = std::make_shared<TransformComponent>(entity->GetId());
			component->setPosition(glm::vec3(pos[0], pos[1], pos[2]));
			component->setScale(glm::vec3(sc[0], sc[1], sc[2]));
			component->setRotation(glm::vec3(rot[0], rot[1], rot[2]));
			ComponentManager::AddComponent<TransformComponent>(component);

		}

		if (exists(j, "Render Component"))
		{
			auto renderComponent = j["Render Component"];
			std::string model = renderComponent["Model"];
			std::shared_ptr<Model> m_model = AssetManager::getModel(model);


			auto materials = renderComponent["Materials"];
			for (auto m_material : materials)
			{
				std::string m_Name = m_material["Material Name"];
				std::string shader = m_material["Shader"];
				std::shared_ptr<Shader> mShader = AssetManager::getShader(shader);
				std::shared_ptr<Material> material = std::make_shared<Material>(m_Name,mShader);

				auto uniforms = m_material["MaterialUniforms"];
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

				auto uniformSamples = m_material["MaterialSampleUniforms"];
				for (auto sample : uniformSamples)
				{
					SHADER_TYPE shaderType = (SHADER_TYPE)sample["Type"];
					switch (shaderType)
					{
					case SHADER_TYPE_SAMPLER2D:
					{
						material->ChangeTexture(sample["Name"], AssetManager::getTexture2D(sample["Texture"]), (int)sample["Unit"]);
						break;
					}
					FRACTURE_ERROR("Unrecognized Uniform type set");
					break;
					}
				}

				AssetManager::AddMaterial(m_Name, material);


				if (AssetManager::getMaterial("DefaultMaterial"))
				{					
					m_model->SetMaterial("DefaultMaterial", material);
				}

			
			}
			
			std::shared_ptr<RenderComponent> component = std::make_shared<RenderComponent>(entity->GetId(), m_model);
			ComponentManager::AddComponent<RenderComponent>(component);

		}
		
		if (exists(j, "Box Collider Component"))
		{
			auto boxColliderComponent = j["Box Collider Component"];
			std::array<float, 3>dimensions = boxColliderComponent["Dimensions"];

			std::shared_ptr<BoxColliderComponent> component = std::make_shared<BoxColliderComponent>(entity->GetId(),
				dimensions[0],
				dimensions[1],
				dimensions[2]);
			ComponentManager::AddComponent<BoxColliderComponent>(component);
		}

		if (exists(j, "Rigidbody Component"))
		{
			auto rigidbodyComponent = j["Rigidbody Component"];
			float mass = rigidbodyComponent["Mass"];
			std::shared_ptr<RigidBodyComponent> component = std::make_shared<RigidBodyComponent>(entity->GetId(), mass);
			ComponentManager::AddComponent<RigidBodyComponent>(component);
		}
		
		if (exists(j, "Light Component"))
		{
			auto lightComponent = j["Light Component"];
			

			LightType lType = (LightType)j["Light Type"];

			switch (lType)
			{
			case LightType::Sun:
			{											
				std::shared_ptr<LightComponent> component = std::make_shared<LightComponent>(entity->GetId(),lType);
				std::array<float, 3> direction = lightComponent["Direction"];
				std::array<float, 4> diffuse = lightComponent["Diffuse"];
				std::array<float, 4> ambient = lightComponent["Ambient"];
				std::array<float, 4> specular = lightComponent["Specular"];
				component->SetDirection(glm::vec3(direction[0], direction[1], direction[2]));
				component->SetAmbient(glm::vec4(ambient[0], ambient[1], ambient[2], ambient[3]));
				component->SetDiffuse(glm::vec4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]));
				component->SetSpecular(glm::vec4(specular[0], specular[1], specular[2], specular[3]));		
				component->SetIntensity(lightComponent["Intensity"]);
				ComponentManager::AddComponent<LightComponent>(component);
				ComponentManager::AddComponent<TransformComponent>(entity->GetId());
				break;
			}
			case LightType::Spot:
			{
				std::array<float, 3> direction = lightComponent["Direction"];
				std::array<float, 3> position = lightComponent["Position"];
				std::array<float, 4> diffuse = lightComponent["Diffuse"];
				std::array<float, 4> ambient = lightComponent["Ambient"];
				std::array<float, 4> specular = lightComponent["Specular"];
				std::shared_ptr<LightComponent> component = std::make_shared<LightComponent>(entity->GetId(), lType);
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
				ComponentManager::AddComponent<TransformComponent>(entity->GetId());
				break;
			}
			case LightType::Point:
			{				
				std::array<float, 3> position = lightComponent["Position"];
				std::shared_ptr<LightComponent> component = std::make_shared<LightComponent>(entity->GetId(), lType);
				std::array<float, 4> diffuse = lightComponent["Diffuse"];
				std::array<float, 4> ambient = lightComponent["Ambient"];
				std::array<float, 4> specular = lightComponent["Specular"];
				component->SetAmbient(glm::vec4(ambient[0], ambient[1], ambient[2], ambient[3]));
				component->SetDiffuse(glm::vec4(diffuse[0], diffuse[1], diffuse[2], diffuse[3]));
				component->SetSpecular(glm::vec4(specular[0], specular[1], specular[2], specular[3]));				
				component->SetPosition(glm::vec3(position[0], position[1], position[2]));
				component->SetLinear(lightComponent["Linear"]);
				component->SetConstant(lightComponent["Constant"]);
				component->SetQuadratic(lightComponent["Qaudratic"]);			
				ComponentManager::AddComponent<LightComponent>(component);
				ComponentManager::AddComponent<TransformComponent>(entity->GetId());
				break;
			}
			case LightType::Sky:
			{
				std::shared_ptr<LightComponent> component = std::make_shared<LightComponent>(entity->GetId(), lType);
				component->SetIntensity(lightComponent["Intensity"]);
				component->ChangeEnvironment(lightComponent["Environment"]);
				ComponentManager::AddComponent<LightComponent>(component);
				ComponentManager::AddComponent<TransformComponent>(entity->GetId());
				break;
			}
			}
		}
		

		m_scene->addEntity(entity);
	}
}
