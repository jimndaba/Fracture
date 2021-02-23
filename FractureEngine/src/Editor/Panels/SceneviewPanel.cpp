#include "SceneviewPanel.h"
#include "Scene/Scene.h"
#include "Entity/EntityFactory.h"

std::shared_ptr<Fracture::Scene> Fracture::SceneView::m_scene;

Fracture::SceneView::SceneView(std::string name):Panel(name)
{
}

Fracture::SceneView::~SceneView()
{
}

void Fracture::SceneView::setScene(std::shared_ptr<Scene> scene)
{
	m_scene = scene;
	m_selection = m_scene->Root();
}

void Fracture::SceneView::render()
{	

	float available_width = ImGui::GetContentRegionAvailWidth();
	ImGui::Columns(2,"scene");


	ImGui::SetColumnWidth(0, available_width - 30);


	DrawEntityNode(m_scene->Root()->GetId());
	
	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_selection = {};

	}
		
	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::MenuItem("Create Empty Entity"))
		{
			std::shared_ptr<Entity> empty = EntityManager::CreateEntity<Entity>();
			ComponentManager::AddComponent<TagComponent>(empty->GetId(), "Empty");
			std::shared_ptr<RelationShipComponent> relation = std::make_shared<RelationShipComponent>(empty->GetId());
			ComponentManager::AddComponent<RelationShipComponent>(relation);

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(empty);
				setSelectEntity(empty);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(empty);
				setSelectEntity(empty);
			}
			
		}
		if (ImGui::MenuItem("Create PointLight"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreatePointlight(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}


		
		}
		if (ImGui::MenuItem("Create SpotLight"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreateSpotlight(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}

		}
		if (ImGui::MenuItem("Create Sphere"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreateSphere(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}

			
		}
		if (ImGui::MenuItem("Create Cube"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreateCube(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}

			
		}
		if (ImGui::MenuItem("Create Plane"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreatePlane(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}

			
		}
		if (ImGui::MenuItem("Create Suzane"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreateSuzane(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}

			
		}
		if (ImGui::MenuItem("Create Cylinder"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreateCylinder(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}

		
		}
		if (ImGui::MenuItem("Create Torus"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreateTorus(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}

		}
		if (ImGui::MenuItem("Create Camera"))
		{
			std::shared_ptr<Entity> entity = EntityFactory::CreateCamera(m_scene);
			std::shared_ptr<RelationShipComponent> relation = ComponentManager::GetComponent<RelationShipComponent>(entity->GetId());

			if (m_selection != nullptr)
			{
				relation->ChangeParent(m_selection->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}
			else
			{
				relation->ChangeParent(m_scene->Root()->GetId());
				m_scene->addEntity(entity);
				setSelectEntity(entity);
			}

			
		}
		
		ImGui::EndPopup();
	}
}

std::shared_ptr<Fracture::Entity> Fracture::SceneView::SelectedEntity()
{
	if(m_selection)
		return m_selection;
	return nullptr;
}

void Fracture::SceneView::setSelectEntity(std::shared_ptr<Entity> selection)
{
	if(selection)
		m_selection = selection;
}

void Fracture::SceneView::clearSelection()
{
	m_selection = {};
}

void Fracture::SceneView::DuplicateSelection(const std::shared_ptr<Entity>& selection)
{
	//scene->Duplicate(selection);
}


void Fracture::SceneView::DrawEntityNode(uint32_t entity)
{
	 
		ImVec2 size = ImVec2(16.0f,16.0f);
		static bool drag_and_drop = false;
		std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity);
		if (ComponentManager::HasComponent<RelationShipComponent>(entity) && tag)
		{
			ImTextureID icon;
			ImGuiTreeNodeFlags flags;
			std::shared_ptr<RelationShipComponent> relationship = ComponentManager::GetComponent<RelationShipComponent>(entity);
			
			flags = ((m_selection && m_selection->GetId() == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

			if (relationship->GetChildren().size() == 0)
			{
				flags |= ImGuiTreeNodeFlags_Leaf;
			}

			if (ComponentManager::HasComponent<CameraControllerComponent>(entity))
			{
				icon = (void*)AssetManager::getTexture2D("CameraIcon")->GetTextureID();
			}
			else if (ComponentManager::HasComponent<LightComponent>(entity))
			{
				icon = (void*)AssetManager::getTexture2D("LightIcon")->GetTextureID();
			}
			else
			{
				icon = (void*)AssetManager::getTexture2D("GameObjectIcon")->GetTextureID();
			}

			ImGui::Image(icon, size);
			ImGui::SameLine();
			bool opened = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag->Name.c_str());

			if (ImGui::IsItemClicked())
			{
				auto selection = m_scene->GetEntity(entity);
				setSelectEntity(selection);					
			}

			bool entityDeleted = false;
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete Entity"))
				{
					if (m_selection != NULL)
					{
						entityDeleted = true;
					}
				}
				ImGui::EndPopup();
			}

			if (entityDeleted)
			{
				m_scene->Destroy(entity);

				if (m_selection->GetId() == entity)
					m_selection = {};

			}

			if (ImGui::BeginDragDropSource())
			{
				ImGui::SetDragDropPayload("_TREENODE", NULL, 0);
				ImGui::Text(tag->Name.c_str());
				ImGui::EndDragDropSource();
			}

			ImGui::NextColumn();
			ImGui::SetColumnWidth(1, 30);
			ImGui::PushID(entity);

			if (tag->isVisible == true)
			{
				ImGui::ImageButton((void*)AssetManager::getTexture2D("EyeIcon")->GetTextureID(), size, ImVec2(0, 0), ImVec2(1, 1), 1);
				if (ImGui::IsItemClicked(0))
				{
					tag->isVisible = !tag->isVisible;
				}
			}
			else
			{
				if (ImGui::ImageButton((void*)AssetManager::getTexture2D("EyeIconC")->GetTextureID(), size, ImVec2(0, 0), ImVec2(1, 1), 1))
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
					ImGui::TreePop();
				}
			
			}


		
		}

}
