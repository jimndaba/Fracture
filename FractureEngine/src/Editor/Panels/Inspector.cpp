#include "Inspector.h"
#include "../Elements/TagComponentElement.h"
#include "SceneviewPanel.h"


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
					DrawVec3Control("Direction", direction);
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
						ComponentManager::AddComponent<RenderComponent>(entity.Id, modelName, "default");
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
			ComponentManager::AddComponent<BoxColliderComponent>(entity.Id, 1.0f, 1.0f, 1.0f);
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Rigidbody"))
		{
			ComponentManager::AddComponent<RigidBodyComponent>(entity.Id, 1.0f);
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

void Fracture::InspectorPanel::DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
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
	ImGui::DragFloat("##X", &value, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::Columns(1);

	ImGui::PopID();
}
