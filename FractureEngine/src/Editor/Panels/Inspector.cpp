#include "Inspector.h"
#include "../Elements/TagComponentElement.h"
#include "SceneviewPanel.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"


Fracture::InspectorPanel::InspectorPanel(std::string name):Panel(name)
{
	
}

Fracture::InspectorPanel::~InspectorPanel()
{
}

void Fracture::InspectorPanel::render()
{
	if (SceneView::SelectedEntity())
	{
		DrawComponents(SceneView::SelectedEntity());
	}
}

void Fracture::InspectorPanel::DrawComponents(Entity entity)
{
	ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

	if (ComponentManager::HasComponent<TagComponent>(entity.Id))
	{
		std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity.Id);

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag->Name.c_str());

		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{	
			tag->Name = std::string(buffer);
		}		
	}

	ImGui::PushItemWidth(-1);
	ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);	

	if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
	{
		ImGui::OpenPopup("ComponentSettings");
	}	
	
	ImGui::PopItemWidth();


	DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			std::shared_ptr<TransformComponent> transform = std::dynamic_pointer_cast<TransformComponent>(component);	
			DrawVec3Control("Position", transform->Position);
			DrawVec3Control("Scale", transform->Scale, 1);
			DrawVec3Control("Rotation", transform->Rotation);
			if (ComponentManager::HasComponent<RigidBodyComponent>(transform->EntityID))
			{
				std::shared_ptr<RigidBodyComponent> body = ComponentManager::GetComponent<RigidBodyComponent>(transform->EntityID);
				body->setPosition(transform->Position);
				body->setRotation(transform->Rotation);
			}


		});

	DrawComponent<EditorNode>("Editor Node", entity, [](auto& component)
		{
			std::shared_ptr<EditorNode> m_node = std::dynamic_pointer_cast<EditorNode>(component);
			glm::vec3 position = m_node->GetPosition();			
			glm::vec3 scale = m_node->GetScale();
			glm::vec3 rotation = m_node->GetRotation();

			if (ComponentManager::HasComponent<LightComponent>(m_node->EntityID))
			{			
				std::shared_ptr<LightComponent> light = ComponentManager::GetComponent<LightComponent>(m_node->EntityID);
				m_node->SetPosition(light->GetPosition());
				m_node->SetScale(glm::vec3(1.0f));
				m_node->SetRotation(light->GetDirection());
			}
			
			
			DrawVec3Control("Position", position);
			DrawVec3Control("Scale", scale, 1);
			DrawVec3Control("Rotation",rotation);
			m_node->SetPosition(position);
			m_node->SetScale(scale);
			m_node->SetRotation(rotation);

			if (ComponentManager::HasComponent<LightComponent>(m_node->EntityID))
			{
				std::shared_ptr<LightComponent> light = ComponentManager::GetComponent<LightComponent>(m_node->EntityID);
				if (light)
				{
					light->SetDirection(m_node->GetRotation());
					light->SetPosition(m_node->GetPosition());
				}				
			}
			
			if (ComponentManager::HasComponent<RigidBodyComponent>(m_node->EntityID))
			{
				std::shared_ptr<RigidBodyComponent> body = ComponentManager::GetComponent<RigidBodyComponent>(m_node->EntityID);
				body->setPosition(m_node->GetPosition());
				body->setRotation(m_node->GetRotation());
			}


		});

	DrawComponent<CameraControllerComponent>("Camera Controller",entity,[](auto& component)
	{
			std::shared_ptr<CameraControllerComponent> camera = std::dynamic_pointer_cast<CameraControllerComponent>(component);
					
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			//const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", "Perspective"))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = false;// = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						//currentProjectionTypeString = projectionTypeStrings[i];
						//camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}

				ImGui::EndCombo();
			}

			float perspectiveVerticalFov = camera->foV;
			if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
			{
				camera->foV = perspectiveVerticalFov;
			}
			
			float perspectiveNear = camera->nearClip;
			if (ImGui::DragFloat("Near", &perspectiveNear))
			{
				camera->nearClip = perspectiveNear;
			}				

			float perspectiveFar = camera->farClip;
			if (ImGui::DragFloat("Far", &perspectiveFar))
			{
				camera->farClip = perspectiveFar;
			}

			DrawVec3Control("Position", camera->Position);
			DrawVec3Control("Front", camera->Front);
			DrawVec3Control("Up", camera->Up);
			DrawVec3Control("Right", camera->Right);
				
	});

	DrawComponent<RenderComponent>("Mesh Render", entity, [](auto& component)
	{

			std::shared_ptr<RenderComponent> render = std::dynamic_pointer_cast<RenderComponent>(component);
			std::string current_Model = render->model->Name;
			std::string current_Material = render->material->Name;
			std::string current_Shader = render->material->getShader()->Name;

			ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

			if (ImGui::BeginCombo("Model",current_Model.c_str()))
			{				
				for (auto const& model : AssetManager::GetModels())
				{
					const bool is_selected = (current_Model.c_str() == model.first.c_str());
					if (ImGui::Selectable(model.first.c_str(), is_selected))
					{
						current_Model = model.first;	
						render->SetModel(model.first);
					}

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
						//render->SetModel(model.first);
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Material", current_Material.c_str()))
			{

				for (auto const& material : AssetManager::GetMaterials())
				{
					bool is_selected = (current_Material.c_str() == material.first.c_str());

					if (ImGui::Selectable(material.first.c_str(), is_selected))
					{
						current_Material = material.first;
						render->SetMaterial(material.first);
					}

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Shader", current_Shader.c_str()))
			{

				for (auto const& shader: AssetManager::GetShaders())
				{
					bool is_selected = (current_Shader.c_str() == shader.first.c_str());

					if (ImGui::Selectable(current_Shader.c_str(), is_selected))
					{
						current_Shader = shader.first;
						render->material->setShader(shader.first);
					}

					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();						
					}
				}
				ImGui::EndCombo();
			}

			ImGui::Separator();

			bool isTransparent = render->material->IsTransparent();
			DrawBoolControl("Is Transparent", isTransparent);
			render->material->setIsTransparent(isTransparent);

			ImGui::Separator();

			bool castShadows = render->material->CastShadows();
			DrawBoolControl("Cast Shadows", castShadows);
			render->material->setCastShadows(castShadows);

			ImGui::Separator();

			auto uniforms = render->material->GetUniforms();
			for (auto value = uniforms->begin(); value != uniforms->end(); ++value)
			{
				DrawMaterialUniform(value->first, value->second);
			}
			ImGui::Separator();
			std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>* uniformsSamplers = render->material->GetSamplerUniforms();
			for (auto it = uniformsSamplers->begin(); it != uniformsSamplers->end(); ++it)
			{
				DrawTexture2DControl(it->first, it->second->texture->id);
			}

			ImGui::Separator();
	});	

	DrawComponent<RigidBodyComponent>("Rigidbody", entity, [](auto& component)
		{
			ImGuiIO& io = ImGui::GetIO();
			auto boldFont = io.Fonts->Fonts[0];
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			std::shared_ptr<RigidBodyComponent> rigid = std::dynamic_pointer_cast<RigidBodyComponent>(component);
			ImVec2 buttonSize = { lineHeight + 15.0f, lineHeight };
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Mass", buttonSize))
			{
				rigid->Mass = 1.0f;
			}				
			ImGui::PopFont();
			ImGui::PopStyleColor(3);
			ImGui::SameLine();
			ImGui::DragFloat("##Mass", &rigid->Mass, 0.1f, 0.0f, 0.0f, "%.2f");
	});

	DrawComponent<BoxColliderComponent>("Collider", entity, [](auto& component)
	{
			std::shared_ptr<BoxColliderComponent> collider = std::dynamic_pointer_cast<BoxColliderComponent>(component);
			glm::vec3 size = glm::vec3(collider->X, collider->Y, collider->Z);
			DrawVec3Control("Size",size,1);
			collider->setScale(size);		
	});

	DrawComponent<ScriptComponent>("Script", entity, [](auto& component)
	{

	});

	DrawComponent<LightComponent>("Light", entity, [](auto& component)
	{
			
			std::shared_ptr<LightComponent> light = std::dynamic_pointer_cast<LightComponent>(component);
			const char* lType = "None";

			if (light->GetLightType() == LightType::Sun)
			{
				lType = "Sunlight";
			}
			if (light->GetLightType() == LightType::Point)
			{
				lType = "Point light";
			}
			if (light->GetLightType() == LightType::Spot)
			{
				lType = "Spotlight";
			}
			static ImGuiComboFlags flags = 0;
			const char* items[] = { "Sunlight","Pointlight","Spotlight" };
			LightType lighttypes[] = {LightType::Sun,LightType::Point ,LightType::Spot };
			static int item_current_idx = 0;                    // Here our selection data is an index.
			static LightType m_LightType = LightType::Point;
			const char* combo_label = items[item_current_idx];  // Label to preview before opening the combo (technically it could be anything)
			if (ImGui::BeginCombo("Light Type", combo_label, flags))
			{
				for (int n = 0; n < IM_ARRAYSIZE(items); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(items[n], is_selected))
					{
						m_LightType = lighttypes[n];
						light->ChangeLightType(m_LightType);
						item_current_idx = n;
					}			

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
					{						
						ImGui::SetItemDefaultFocus();
					}
						
				}
				ImGui::EndCombo();
			}

			switch(light->GetLightType())
			{
				case LightType::Sun:
				{
					glm::vec3 direction = light->GetDirection();
					glm::vec4 ambient = light->GetAmbient();
					glm::vec4 diffuse = light->GetDiffuse();
					glm::vec4 specular = light->GetSpecular();

					DrawVec3Control("Direction", direction);					
					DrawColourControl("Ambient", ambient, 1.0f);					
					DrawColourControl("Diffuse", diffuse, 1.0f);					
					DrawColourControl("Specular", specular, 1.0f);

					light->SetDirection(direction);
					light->SetAmbient(ambient);
					light->SetDiffuse(diffuse);
					light->SetSpecular(specular);
					break;
				}
				case LightType::Point:
				{
					glm::vec3 position = light->GetPosition();
					DrawVec3Control("Position",position );
					light->SetPosition(position);

					glm::vec4 ambient = light->GetAmbient();
					DrawColourControl("Ambient", ambient, 1.0f);
					light->SetAmbient(ambient);
					glm::vec4 diffuse = light->GetDiffuse();
					DrawColourControl("Diffuse", diffuse, 1.0f);
					light->SetDiffuse(diffuse);
					glm::vec4 specular = light->GetSpecular();
					DrawColourControl("Specular", specular, 1.0f);
					light->SetSpecular(specular);

					float constant = light->GetConstant();
					DrawfloatControl("Constant", constant, 1.0f);
					light->SetConstant(constant);

					float linear = light->GetLinear();
					DrawfloatControl("Linear", linear, 1.0f);
					light->SetLinear(linear);

					float quad = light->GetQuadratic();
					DrawfloatControl("Quadratic", quad, 1.0f);
					light->SetQuadratic(quad);
					break;
				}
				case LightType::Spot:
				{
					glm::vec3 position = light->GetPosition();
					glm::vec3 direction = light->GetDirection();
					DrawVec3Control("Position", position);
					DrawVec3Control("Direction", direction);
					light->SetPosition(position);
					light->SetDirection(direction);

					glm::vec4 ambient = light->GetAmbient();
					DrawColourControl("Ambient", ambient, 1.0f);
					light->SetAmbient(ambient);
					glm::vec4 diffuse = light->GetDiffuse();
					DrawColourControl("Diffuse", diffuse, 1.0f);
					light->SetDiffuse(diffuse);
					glm::vec4 specular = light->GetSpecular();
					DrawColourControl("Specular", specular, 1.0f);
					light->SetSpecular(specular);

					float constant = light->GetConstant();
					DrawfloatControl("Constant", constant, 1.0f);
					light->SetConstant(constant);

					float linear = light->GetLinear();
					DrawfloatControl("Linear", linear, 1.0f);
					light->SetLinear(linear);

					float quad = light->GetQuadratic();
					DrawfloatControl("Quadratic", quad, 1.0f);
					light->SetQuadratic(quad);

					float cutoff = light->GetCutoff();
					DrawfloatControl("Cutoff", cutoff, 1.0f);
					light->SetCutoff(cutoff);

					float ocutoff = light->GetOuterCutOff();
					DrawfloatControl("OuterCutoff", ocutoff, 1.0f);
					light->SetOuterCutOff(ocutoff);
					break;
				}
			}
	});

	ImGui::Separator();

	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("Transform"))
		{
			ComponentManager::AddComponent<TransformComponent>(entity.Id);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Editor Node"))
		{
			ComponentManager::AddComponent<EditorNode>(entity.Id);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Camera"))
		{
			ComponentManager::AddComponent<CameraControllerComponent>(entity.Id);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::BeginMenu("Render"))
		{
			std::string modelName;
			for (auto const& model : AssetManager::GetModels())
			{
				if (ImGui::MenuItem(model.first.c_str()))
				{
					modelName = model.first;
					if (ComponentManager::HasComponent<TransformComponent>(entity.Id))
					{
						std::string mat_name = modelName + "Materail";
						std::shared_ptr<Material> material = std::make_shared<Material>(mat_name, AssetManager::getShader("default"));
						AssetManager::AddMaterial(mat_name,material);
						ComponentManager::AddComponent<RenderComponent>(entity.Id, modelName, mat_name);
					}
					else
					{
						FRACTURE_ERROR("ENTITY DOES NOT HAVE TRANSFORM COMPONENT");
					}

				}
			}
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Audio"))
		{
			//ComponentManager::AddComponent<CameraControllerComponent>(entity.Id);			
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Light"))
		{
			if (ComponentManager::HasComponent<CameraControllerComponent>(entity.Id))
			{
				FRACTURE_ERROR("Cannot Attach Light to Entity with Camera Component!");

			}
			else
			{
				ComponentManager::AddComponent<LightComponent>(entity.Id, LightType::Sun);
			}
			
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Collision"))
		{
			if (ComponentManager::HasComponent<CameraControllerComponent>(entity.Id))
			{
				FRACTURE_ERROR("Cannot Attach Light to Entity with Camera Component!");

			}
			else
			{
				ComponentManager::AddComponent<BoxColliderComponent>(entity.Id, 1.0f, 1.0f, 1.0f);
			}		
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Rigidbody"))
		{
			if (ComponentManager::HasComponent<CameraControllerComponent>(entity.Id))
			{
				FRACTURE_ERROR("Cannot Attach Light to Entity with Camera Component!");
			}
			else if (!ComponentManager::HasComponent<BoxColliderComponent>(entity.Id))
			{
				ComponentManager::AddComponent<BoxColliderComponent>(entity.Id, 1.0f,1.0f,1.0f);
				ComponentManager::AddComponent<RigidBodyComponent>(entity.Id, 1.0f);
			}
			else
			{
				ComponentManager::AddComponent<RigidBodyComponent>(entity.Id, 1.0f);
			}
			
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Script"))
		{
			//ComponentManager::AddComponent<ScriptComponent>(entity.Id);
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}

}

void Fracture::InspectorPanel::DrawTextInputControl(const std::string& label, std::string& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImGui::PushID(label.c_str());
	
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy_s(buffer, sizeof(buffer), values.c_str());	
	if (ImGui::InputText(label.c_str(), buffer, sizeof(buffer)))
	{
		std::string str(buffer);
		values = str;
	}
	
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::InspectorPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::AlignTextToFramePadding();
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	
	
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

void Fracture::InspectorPanel::DrawMaterialUniform(const std::string& label, UniformValue& value, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;

	auto boldFont = io.Fonts->Fonts[0];
	ImGui::PushID(label.c_str());
	
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strcpy_s(buffer, sizeof(buffer), value.Name.c_str());
	ImGui::PushItemWidth(-1);
	if (ImGui::InputText("##uniformText", buffer, sizeof(buffer),flags))
	{
		std::string str(buffer);
		value.Name = str;
	}
	ImGui::PopItemWidth();	
	ImGui::NextColumn();


	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
	ImGui::PushFont(boldFont);
	if (ImGui::Button("-", buttonSize))
		value.Float = resetValue;
	ImGui::PopFont();

	ImGui::SameLine();
	switch (value.Type)
	{
	case SHADER_TYPE_BOOL:
	{		
		ImGui::Checkbox(label.c_str(), &value.Bool);

		break;
	}		
	case SHADER_TYPE_INT:
	{
		ImGui::InputInt("##X", &value.Int);
		break;
	}
	case SHADER_TYPE_FLOAT:
	{
		ImGui::DragFloat("##uniform", &value.Float, 0.1f, 0.0f, 0.0f, "%.2f");
	
		break;
	}
	case SHADER_TYPE_VEC2:
	{
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value.Vec2.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.Vec2.x, 0.05f, 0.0f, 1.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			value.Vec2.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.Vec2.y, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		break;
	}
	case SHADER_TYPE_VEC3:
	{
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value.Vec3.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.Vec3.x, 0.05f, 0.0f, 1.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			value.Vec3.y= resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.Vec3.y, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();
		
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			value.Vec3.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.Vec3.z, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
	
		break;
	}
	case SHADER_TYPE_VEC4:
	{
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value.Vec4.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.Vec4.x, 0.05f, 0.0f, 1.0f, "%.2f");

		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			value.Vec4.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.Vec4.y, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			value.Vec4.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.Vec4.z, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.84f, 0.84f, 0.84f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.95f, 0.95f, 0.95f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGui::Button("w", buttonSize))
			value.Vec4.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##w", &value.Vec4.w, 0.05f, 0.0f, 1.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
	
		break;
	}
	case SHADER_TYPE_MAT2:
	{
		//ImGui::SameLine();
		//DrawBoolControl(value.Name, value.Bool);
		break;
	}
	case SHADER_TYPE_MAT3:
	{
		//ImGui::SameLine();
		//DrawBoolControl(value.Name, value.Bool);
		break;
	}
	case SHADER_TYPE_MAT4:
	{
		//ImGui::SameLine();
		//DrawBoolControl(value.Name, value.Bool);
		break;
	}
	default:
		//Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
		break;
	}

	
	ImGui::Columns(1);
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::InspectorPanel::DrawColourControl(const std::string& label, glm::vec4& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	float color4[4] = { values .x,values.y,values.z,values.w };
	ImGui::PushID("##color");		
	ImGui::ColorEdit4(label.c_str(), (float*)&color4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);	
	values = glm::vec4(color4[0], color4[1], color4[2], color4[3]);
	ImGui::SameLine();
	ImGui::Text(label.c_str());
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::InspectorPanel::DrawfloatControl(const std::string& label, float& value, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushFont(boldFont);
	if (ImGui::Button("-", buttonSize))
		value = resetValue;
	ImGui::PopFont();
	ImGui::SameLine();
	ImGui::DragFloat("##uniform", &value, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::Columns(1);

	ImGui::PopID();
}

void Fracture::InspectorPanel::DrawIntControl(const std::string& label, int& value, int resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushFont(boldFont);
	if (ImGui::Button("-", buttonSize))
		value = resetValue;
	ImGui::PopFont();
	ImGui::SameLine();
	ImGui::InputInt("##X", &value);
	ImGui::Columns(1);

	ImGui::PopID();
}

void Fracture::InspectorPanel::DrawBoolControl(const std::string& label, bool& value, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImGui::PushID("##bool");
	ImGui::PushFont(boldFont);
	ImGui::Checkbox(label.c_str(), &value);
	ImGui::PopFont();
	ImGui::SameLine();
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::InspectorPanel::DrawTexture2DControl(const std::string& label,unsigned int& value, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImTextureID texture = (void*)value;
	ImGui::PushID("##sample");
	ImGui::ImageButton(texture, ImVec2(64, 64));
	ImGui::SameLine();
	ImGui::Text(label.c_str());
	ImGui::PopID();
	ImGui::Separator();
}
