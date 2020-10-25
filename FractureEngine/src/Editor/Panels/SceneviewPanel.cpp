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
	
	for (auto& entity : m_scene->Entities())
	{
		DrawEntityNode(*entity.get());
	}


	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		m_selection = {};

	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			std::shared_ptr<Entity> empty = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(empty->Id, "Empty");
			m_scene->addEntity(empty);
		}
			

		ImGui::EndPopup();
	}
}

Fracture::Entity Fracture::SceneView::SelectedEntity()
{
	return m_selection;
}

void Fracture::SceneView::DrawEntityNode(Entity entity)
{
	std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity.Id);

	ImGuiTreeNodeFlags flags = ((m_selection.Id == entity.Id) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity.Id, flags, tag->Name.c_str());
	if (ImGui::IsItemClicked())
	{
		m_selection = entity;
	}

	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag->Name.c_str());
		if (opened)
			ImGui::TreePop();
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_scene->Destroy(entity.Id);
		if (m_selection.Id == entity.Id)
			m_selection = {};
	}



}
