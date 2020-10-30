#include "SceneviewPanel.h"
#include "Scene/Scene.h"

Fracture::Entity Fracture::SceneView::m_selection;

Fracture::SceneView::SceneView(std::string name):Panel(name)
{
}

Fracture::SceneView::~SceneView()
{
}

void Fracture::SceneView::setScene(std::shared_ptr<Scene> scene)
{
	m_scene = scene;
	m_selection = NULL;
}

void Fracture::SceneView::render()
{	

	float available_width = ImGui::GetContentRegionAvailWidth();
	ImGui::Columns(2,"scene");
	
	ImGui::SetColumnWidth(0, available_width - 30);


	DrawEntityNode(m_scene->Root()->Id);
	
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) //
		m_selection = NULL;

	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			std::shared_ptr<Entity> empty = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(empty->Id, "Empty");
			std::shared_ptr<RelationShipComponent> relation = std::make_shared<RelationShipComponent>(empty->Id);
			ComponentManager::AddComponent(relation);
			if (m_selection.Id != NULL)
			{
				relation->SetParent(m_selection.Id);
				m_scene->addEntity(empty);
			}
			else
			{
				relation->SetParent(m_scene->Root()->Id);
				m_scene->addEntity(empty);
			}
			
		}
		ImGui::EndPopup();
	}
}

Fracture::Entity Fracture::SceneView::SelectedEntity()
{
	return m_selection;
}

void Fracture::SceneView::DrawEntityNode(uint32_t entity)
{
	ImVec2 size = ImVec2(16.0f,16.0f);
	std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity);

	

	if (ComponentManager::HasComponent<RelationShipComponent>(entity) && tag)
	{
		ImTextureID icon;
		std::shared_ptr<RelationShipComponent> relationship = ComponentManager::GetComponent<RelationShipComponent>(entity);
		ImGuiTreeNodeFlags flags = ((m_selection.Id == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (relationship->GetChildren().size() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (ComponentManager::HasComponent<CameraControllerComponent>(entity))
		{
			icon = (void*)AssetManager::getTexture("CameraIcon")->id;
		}
		else 
		{
			icon = (void*)AssetManager::getTexture("GameObjectIcon")->id;
		}
		
		ImGui::Image(icon, size);
		ImGui::SameLine();
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag->Name.c_str());
		
	
		
		if (ImGui::IsItemClicked())
		{
			m_selection.Id = entity;
		}
		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;
			ImGui::EndPopup();
		}

		if (entityDeleted)
		{
			if (relationship->GetChildren().size() > 0)
			{
				for (auto& child : relationship->GetChildren())
				{
					m_scene->Destroy(child);
				}
			}
			m_scene->Destroy(entity);
			
			if (m_selection.Id == entity)
				m_selection = {};
		}
		
		ImGui::NextColumn();
		ImGui::SetColumnWidth(1, 30);
		ImGui::PushID(entity);
		
		if (tag->isVisible == true)
		{						
			ImGui::ImageButton((void*)AssetManager::getTexture("EyeIcon")->id, size, ImVec2(0, 0), ImVec2(1, 1), 1);
			if (ImGui::IsItemClicked(0))
			{
				tag->isVisible = !tag->isVisible;
			}				
		}
		else
		{
			if (ImGui::ImageButton((void*)AssetManager::getTexture("EyeIconC")->id, size, ImVec2(0, 0), ImVec2(1, 1), 1))
			{
				tag->isVisible = true;
			}
				
		}
		ImGui::PopID();
		ImGui::NextColumn();
		//ImGui::ImageButton(nullptr, size, ImVec2(0, 0), ImVec2(1, 1), 1);
		//ImGui::NextColumn();

		if (opened)
		{
			if (ComponentManager::HasComponent<RelationShipComponent>(entity))
			{
				for (auto& child : relationship->GetChildren())
				{

					DrawEntityNode(child);
				}
			}
			ImGui::TreePop();
		}
			
	}
	
}
