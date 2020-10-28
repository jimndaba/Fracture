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
	m_selection = {};
}

void Fracture::SceneView::render()
{	
	ImGui::Columns(3,"scene");
	
	DrawEntityNode(m_scene->Root()->Id);
	
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) //
		m_selection = {};

	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			std::shared_ptr<Entity> empty = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(empty->Id, "Empty");
			std::shared_ptr<RelationShipComponent> relation = std::make_shared<RelationShipComponent>(empty->Id);
			ComponentManager::AddComponent(relation);
			relation->SetParent(m_selection.Id);
			m_scene->addEntity(empty);
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
	std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity);
	if (ComponentManager::HasComponent<RelationShipComponent>(entity) && tag)
	{
		std::shared_ptr<RelationShipComponent> relationship = ComponentManager::GetComponent<RelationShipComponent>(entity);

		ImGuiTreeNodeFlags flags = ((m_selection.Id == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		if (relationship->GetChildren().size() == 0)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}
			
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
		ImGui::ImageButton(nullptr, ImVec2(8, 8), ImVec2(0, 0), ImVec2(1, 1), 1);
		ImGui::NextColumn();
		ImGui::ImageButton(nullptr, ImVec2(8, 8), ImVec2(0, 0), ImVec2(1, 1), 1);
		ImGui::NextColumn();

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
