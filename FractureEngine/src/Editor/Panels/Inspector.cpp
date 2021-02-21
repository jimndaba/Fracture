#include "Inspector.h"
#include "../Elements/TagComponentElement.h"
#include "SceneviewPanel.h"
#include "Rendering/Shader.h"
#include "Rendering/Material.h"
#include "../utils/FileDialogue.h"


Fracture::SampleUniformType stringToEnum(const std::string& m_type);

Fracture::InspectorPanel::InspectorPanel(std::string name, Fracture::SceneView& scenegraph):Panel(name),m_scenegraph(scenegraph)
{
}

Fracture::InspectorPanel::~InspectorPanel()
{
}

void Fracture::InspectorPanel::render()
{
	if (m_scenegraph.SelectedEntity())
	{
		DrawComponents(*m_scenegraph.SelectedEntity().get());
	}
}

void Fracture::InspectorPanel::DrawComponents(Entity entity)
{

	DrawComponent<TagComponent>("Tag", entity, [](auto& component)
	{
		std::shared_ptr<TagComponent> tag = std::dynamic_pointer_cast<TagComponent>(component);

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tag->Name.c_str());

		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag->Name = std::string(buffer);
		}
	});

	DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
		{
			std::shared_ptr<TransformComponent> transform = std::dynamic_pointer_cast<TransformComponent>(component);

			glm::vec3 position = transform->Position();
			glm::vec3 scale = transform->Scale();
			glm::vec3 rotation = transform->Rotation();
			
			DrawVec3Control("Position", position);
			DrawVec3Control("Scale", scale, 1);
			DrawVec3Control("Rotation",rotation);
			
			transform->setPosition(position);
			transform->setScale(scale);
			transform->setRotation(rotation);

			if (ComponentManager::HasComponent<RigidBodyComponent>(transform->EntityID))
			{
				std::shared_ptr<RigidBodyComponent> body = ComponentManager::GetComponent<RigidBodyComponent>(transform->EntityID);
				body->setPosition(transform->Position());
				body->setRotation(transform->Rotation());
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

			ImGuiIO& io = ImGui::GetIO();
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
			auto font = io.Fonts->Fonts[0];

			std::shared_ptr<RenderComponent> render = std::dynamic_pointer_cast<RenderComponent>(component);
			std::string current_Model = render->GetModel()->Name;		
			
			ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

			if (ImGui::BeginCombo("Model",current_Model.c_str(),flags))
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
						render->SetModel(model.first);
					}
				}
				ImGui::EndCombo();
			}
			
			// Model can have more that 1 material
			ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
			
			for (auto material : render->GetModel()->GetMaterials())//
			{
				
				bool open = ImGui::TreeNodeEx(material->Name.c_str(), treeNodeFlags, material->Name.c_str());
				std::string current_Material = material->Name;

				if (open)
				{			
					
					if (ImGui::BeginCombo("##Material", current_Material.c_str()))
					{

						for (auto const& nMaterial : AssetManager::GetMaterials())
						{
							bool is_selected = (current_Material.c_str() == nMaterial.first.c_str());

							if (ImGui::Selectable(nMaterial.first.c_str(), is_selected))
							{
								render->SetMaterial(current_Material, nMaterial.first);
								current_Material = nMaterial.first;
							
							}

							if (is_selected)
							{
								ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}					

					bool isTransparent = material->IsTransparent();
					DrawBoolControl("Transparent", isTransparent);
					material->setIsTransparent(isTransparent);					

					bool castShadows = material->CastShadows();
					DrawBoolControl("CastShadows", castShadows);
					material->setCastShadows(castShadows);

					if (ImGui::Button("reload", ImVec2(100, 20)))
					{
						material->getShader()->reloadShader();

						FRACTURE_INFO("Reloaded Shader: {}", material->getShader()->Name);
					}

					ImGui::Separator();


					if (ImGui::Button("Add Unfiorm", buttonSize))
					{
						ImGui::OpenPopup("Add Uniform?");
					}

					const auto& uniforms = material->GetUniforms();
					for (auto value = uniforms->begin(); value != uniforms->end(); ++value)
					{
						ImGui::Columns(3);
						ImGui::SetColumnWidth(0, 40);						
						ImGui::NextColumn();
						DrawMaterialUniform(value->first, value->second);

						
					}
					ImGui::Separator();
					const auto& uniformsSamplers = material->GetSamplerUniforms();
					for (auto it = uniformsSamplers->begin(); it != uniformsSamplers->end(); ++it)
					{
						if (it->second->Type == SHADER_TYPE::SHADER_TYPE_SAMPLER2D)
						{
							DrawSample2DControl(it->first, it->second->texture->id, material);
						}

					}

					if (ImGui::BeginPopupModal("Add Uniform?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("uniform Name: .\n");
						ImGui::SameLine();
						static std::string name;
						static std::string textureName;
						static std::string m_types[] = {
							"Diffuse",
							"Normal",
							"Metallic",
							"Roughness",
							"AO",
							"Specular",
							"Emmision"
						};

						
						static int item_current_idx = 0;
						std::string current_type = m_types[item_current_idx];
						if (ImGui::BeginCombo("##uniformType",current_type.c_str()))
						{
							for (int n = 0; n < IM_ARRAYSIZE(m_types); n++)
							{
								const bool is_selected = (item_current_idx == n);
								if (ImGui::Selectable(m_types[n].c_str(), is_selected))
								{
									item_current_idx = n;
								}						

								// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
								if (is_selected)
									ImGui::SetItemDefaultFocus();
							}
							ImGui::EndCombo();
						}
						ImGui::SameLine();
						std::shared_ptr<Texture> map;
						if (ImGui::Button("+", buttonSize))
						{

							std::string filepath = FileDialogue::OpenFile("png(*.png)\0*.png\0jpg(*.jpg)\0*.jpg\0bmp(*.bmp)\0*.bmp\0", textureName);
							if (!filepath.empty())
							{
								AssetManager::AddTexture(textureName, filepath, TextureType::Diffuse);
							}
						}

						switch (stringToEnum(current_type))
						{
							case SampleUniformType::Diffuse:
							{
								name = "albedoMap";
							}
							break;
							case SampleUniformType::Normal:
							{
								name = "normalMap";
							}
							break;
							case SampleUniformType::Metallic:
							{
								name = "metallicMap";
							}
							break;
							case SampleUniformType::Roughness:
							{
								name = "roughnessMap";
							}
							break;
							case SampleUniformType::AO:
							{
								name = "aoMap";
							}
							break;
							case SampleUniformType::Specular:
							{
								name = "specularMap";
							}
							break;
						}

						if (ImGui::Button("OK", ImVec2(120, 0)))
						{
							if (!name.empty() && !textureName.empty())
							{
								material->SetTexture(name, AssetManager::getTexture(textureName), (unsigned int)stringToEnum(current_type));
								ImGui::CloseCurrentPopup();
							}
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
						ImGui::EndPopup();
					}
					ImGui::TreePop();
				
				}
			}
	});	

	DrawComponent<AnimatorComponent>("Animator", entity, [](auto& component)
	{
		ImGuiIO& io = ImGui::GetIO();
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 2.0f, lineHeight };
		auto font = io.Fonts->Fonts[0];

		std::shared_ptr<AnimatorComponent> animator = std::dynamic_pointer_cast<AnimatorComponent>(component);
		std::string current_Animation;

		if(animator->m_CurrentAnimation)
			current_Animation = animator->m_CurrentAnimation->Name;

		ImGuiComboFlags flags = ImGuiComboFlags_NoArrowButton;

		if (ImGui::BeginCombo("Animation", current_Animation.c_str(),flags))
		{
			for (auto const& animation : animator->m_animations)
			{
				const bool is_selected = (current_Animation.c_str() == animation.first.c_str());
				if (ImGui::Selectable(animation.first.c_str(), is_selected))
				{
					current_Animation = animation.first;
					animator->SetAnimation(animation.first);
				}

				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
					animator->SetAnimation(animation.first);
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
			if (light->GetLightType() == LightType::Sky)
			{
				lType = "Skylight";
			}

			static ImGuiComboFlags flags = 0;
			const char* items[] = { "Sunlight","Pointlight","Spotlight","SkyLigh" };
			LightType lighttypes[] = {LightType::Sun,LightType::Point ,LightType::Spot,LightType::Sky };
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

			bool shadow = light->CastShadow();
			DrawBoolControl("CastShadows", shadow);
			light->SetCastShadow(shadow);

			float intensity = light->Intensity();
			DrawfloatControl("Intensity",intensity,0.0f,100.0f);
			light->SetIntensity(intensity);

			switch(light->GetLightType())
			{
				case LightType::Sun:
				{
					glm::vec3 direction = light->GetDirection();					
					glm::vec4 diffuse = light->GetDiffuse();					
					glm::vec3 radiance = light->GetRadiance();

					DrawVec3Control("Radiance", radiance);
					DrawVec3Control("Direction", direction);								
					DrawColourControl("Colour", diffuse, 1.0f);			

					light->SetDirection(direction);			
					light->SetDiffuse(diffuse);							
					light->SetRadiance(radiance);
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
				case LightType::Sky:
				{
					ImGui::Text("Change Environment");
					ImGui::SameLine();
					if (ImGui::Button("-", ImVec2{ 30, 30}))
					{
						std::string name;
						std::string filepath = FileDialogue::OpenFile("HDR(*.hdr)\0*.hdr\0", name);
						if (!filepath.empty())
						{
							AssetManager::AddEnvironmentMap(name, filepath);
							light->ChangeEnvironment(name);
						}
					}
					break;
				}
			}
	});
	
	if (ImGui::Button("Add Component"))
	{
		ImGui::OpenPopup("AddComponent");
	}

	if (ImGui::BeginPopup("AddComponent"))
	{
		if (ImGui::MenuItem("Transform"))
		{
			if (m_scenegraph.SelectedEntity())
			{
				ComponentManager::AddComponent<TransformComponent>(entity.Id);
			}			
			ImGui::CloseCurrentPopup();
		}
			

		if (ImGui::MenuItem("Camera"))
		{

			if (m_scenegraph.SelectedEntity())
			{
				ComponentManager::AddComponent<CameraControllerComponent>(entity.Id);
			}
			
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::BeginMenu("Render"))
		{
			if (m_scenegraph.SelectedEntity())
			{
				std::string modelName;
				for (auto const& model : AssetManager::GetModels())
				{
					if (ImGui::MenuItem(model.first.c_str()))
					{
						modelName = model.first;
						if (ComponentManager::HasComponent<TransformComponent>(entity.Id))
						{
							std::string mat_name = model.second->Name;
							std::shared_ptr<Material> material = AssetManager::getMaterial(mat_name);
							if (material)
							{
								//ComponentManager::AddComponent<RenderComponent>(entity.Id, modelName, mat_name);
							}
							else
							{
								//material = std::make_shared<Material>(mat_name, AssetManager::getShader("default"));
								//AssetManager::AddMaterial(mat_name, material);
								//ComponentManager::AddComponent<RenderComponent>(entity.Id, modelName, mat_name);
							}							
							
						}
						else
						{
							FRACTURE_ERROR("ENTITY DOES NOT HAVE TRANSFORM COMPONENT");
						}

					}
				}
				ImGui::EndMenu();
			}
		}

		if (ImGui::MenuItem("Audio"))
		{
			if (m_scenegraph.SelectedEntity())
			{
				//ComponentManager::AddComponent<CameraControllerComponent>(entity.Id);		
			}
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Light"))
		{
			if (m_scenegraph.SelectedEntity())
			{
				if (ComponentManager::HasComponent<CameraControllerComponent>(entity.Id))
				{
					FRACTURE_ERROR("Cannot Attach Light to Entity with Camera Component!");

				}
				else
				{
					ComponentManager::AddComponent<LightComponent>(entity.Id, LightType::Sun);
				}
			}
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Collision"))
		{
			if (m_scenegraph.SelectedEntity())
			{
				if (ComponentManager::HasComponent<CameraControllerComponent>(entity.Id))
				{
					FRACTURE_ERROR("Cannot Attach Light to Entity with Camera Component!");

				}
				else
				{
					ComponentManager::AddComponent<BoxColliderComponent>(entity.Id, 1.0f, 1.0f, 1.0f);
				}
			}
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Rigidbody"))
		{
			if (m_scenegraph.SelectedEntity())
			{
				if (ComponentManager::HasComponent<CameraControllerComponent>(entity.Id))
				{
					FRACTURE_ERROR("Cannot Attach Light to Entity with Camera Component!");
				}
				else if (!ComponentManager::HasComponent<BoxColliderComponent>(entity.Id))
				{
					ComponentManager::AddComponent<BoxColliderComponent>(entity.Id, 1.0f, 1.0f, 1.0f);
					ComponentManager::AddComponent<RigidBodyComponent>(entity.Id, 1.0f);
				}
				else
				{
					ComponentManager::AddComponent<RigidBodyComponent>(entity.Id, 1.0f);
				}
			}
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Script"))
		{
			if (m_scenegraph.SelectedEntity())
			{
				//ComponentManager::AddComponent<ScriptComponent>(entity.Id);
			}
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("Animator"))
		{
			if (m_scenegraph.SelectedEntity())
			{
				ComponentManager::AddComponent<AnimatorComponent>(entity.Id);		
			}
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
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.95f, 0.95f, 0.95f, 1.00f));
	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::SameLine();
	ImGui::PopStyleColor(4);


	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85f, 0.85f, 0.85f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	int push = 2;
	const bool buttonHovered = ImGui::IsItemHovered();
	if (buttonHovered)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.00f));
		push = 3;
	}

	
	ImGui::PopStyleColor(push);
	ImGui::PopFont();
	
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85f, 0.85f, 0.85f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	
	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::PopFont();
	ImGui::PopStyleColor(5);


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.34f, 0.58f, 0.97f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.4f, 0.6f, 1.0f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.34f, 0.58f, 0.97f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85f, 0.85f, 0.85f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.15f, 0.15f, 0.15f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_TextSelectedBg,ImVec4(1.0f, 1.0f, 1.0f, 1.00f));
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::PopFont();
	ImGui::PopStyleColor(6);

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
	
	//ImGui::Columns(2);
	ImGui::SetColumnWidth(1, columnWidth);

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
	switch (value.Type)
	{
	case SHADER_TYPE_BOOL:
	{		
		ImGui::Checkbox(label.c_str(), &value.Bool);

		break;
	}		
	case SHADER_TYPE_INT:
	{
		ImGui::PushFont(boldFont);
		if (ImGui::Button("-", buttonSize))
			value.Int =(int)resetValue;
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::InputInt("##X", &value.Int);
		break;
	}
	case SHADER_TYPE_FLOAT:
	{
		ImGui::PushFont(boldFont);
		if (ImGui::Button("-", buttonSize))
			value.Float = resetValue;
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::DragFloat("##uniform", &value.Float, 0.001f, 0.0f, 1.0f, "%.2f");
	
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
		ImGui::DragFloat("##X", &value.Vec2.x, 0.001f, 0.0f, 1.0f, "%.2f");

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
	case SHADER_TYPE_COLOR3:
	{
		glm::vec4 color = glm::vec4(value.Color3, 1.0f);
		DrawColourControl(label, color, 1.0f);
		value.Color3 = glm::vec3(color.x, color.y, color.z);
		break;
	}
	case SHADER_TYPE_COLOR4:
	{
		DrawColourControl(label,value.Color4,1.0f);
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

void Fracture::InspectorPanel::DrawfloatControl(const std::string& label, float& value, float resetValue, const float& max, float columnWidth)
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
	ImGui::DragFloat("##uniform", &value, 0.01f, 0.0f,max, "%.2f");
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
	if (ImGui::ImageButton(texture, ImVec2(64, 64)) && ImGui::IsMouseDoubleClicked(0))
	{
		std::string name;
		std::string filepath = FileDialogue::OpenFile("png(*.png)\0*.png\0jpg(*.jpg)\0*.jpg\0bmp(*.bmp)\0*.bmp\0", name);
		if (!filepath.empty())
		{
			AssetManager::AddTexture2D(name,filepath,TextureType::Diffuse);
			value = AssetManager::getTexture(name)->GetTextureID();
		}
	}
	ImGui::SameLine();
	ImGui::Text(label.c_str());
	ImGui::PopID();
	ImGui::Separator();
}

void Fracture::InspectorPanel::DrawSample2DControl(const std::string& label, unsigned int& value, std::shared_ptr<Fracture::Material> mMaterial, float resetValue, float columnWidth)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];
	ImTextureID texture = (void*)value;
	ImGui::PushID("##sample");
	if (ImGui::ImageButton(texture, ImVec2(64, 64)))
	{
		std::string name;
		std::string filepath = FileDialogue::OpenFile("png(*.png)\0*.png\0jpg(*.jpg)\0*.jpg\0bmp(*.bmp)\0*.bmp\0", name);
		if (!filepath.empty())
		{
			AssetManager::AddTexture2D(name, filepath, TextureType::Diffuse);
			mMaterial->ChangeTexture(label, AssetManager::getTexture(name), (int)AssetManager::getTexture(name)->GetType());
		}
		
	}
	ImGui::SameLine();
	ImGui::Text(label.c_str());
	ImGui::PopID();
	ImGui::Separator();
}

Fracture::SampleUniformType stringToEnum(const std::string& m_type)
{
	if (m_type == "Diffuse")
	{
		return Fracture::SampleUniformType::Diffuse;
	}
	if (m_type == "Normal")
	{
		return Fracture::SampleUniformType::Normal;
	}
	if (m_type == "Metallic")
	{
		return Fracture::SampleUniformType::Metallic;
	}
	if (m_type == "Roughness")
	{
		return Fracture::SampleUniformType::Roughness;
	}
	if (m_type == "AO")
	{
		return Fracture::SampleUniformType::AO;
	}
	if (m_type == "Specular")
	{
		return Fracture::SampleUniformType::Specular;
	}
	if (m_type == "Emmision")
	{
		return Fracture::SampleUniformType::Emmision;
	}

}