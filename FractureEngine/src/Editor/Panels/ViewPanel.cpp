#include "ViewPanel.h"
#include "Rendering/RenderTarget.h"
#include "SceneviewPanel.h"
#include "Rendering/PickingPass.h"
#include "../Editor.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <Component\EditorNodeComponent.h>

int Fracture::ViewPanel::gizmoMode;


Fracture::ViewPanel::ViewPanel(std::string name):Panel(name)
{
	gizmoMode = 0;
}

Fracture::ViewPanel::~ViewPanel()
{
}

void Fracture::ViewPanel::init()
{
	//m_camera = ComponentManager::GetComponent<CameraControllerComponent>(Editor::ActiveScene()->ActiveCamera()->Id);
}

void Fracture::ViewPanel::setRenderer(Renderer* renderer)
{
    m_renderer = renderer;
	m_camera = m_renderer->ActiveCamera();
}

void Fracture::ViewPanel::render()
{
	ProfilerTimer timer("viewPanel Render");

	/*
	if (ImGui::RadioButton("Move (W)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::TRANSLATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate (E)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::ROTATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale (R)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::SCALE; };
	*/

	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };


	

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::BeginChild("SceneView");	

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x ,  viewportPanelSize.y };
	m_renderer->setViewport(viewportPanelSize.x, viewportPanelSize.y);


	//Draw Screen Picking Window
	//ImGui::Image(reinterpret_cast<void*>(m_renderer->m_PickingPass->m_renderTarget->GetColorTexture(0)->id),
	//	viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	//Draw Final Image from Render Target
    ImGui::Image(reinterpret_cast<void*>(m_renderer->SceneRenderTarget->GetColorTexture(0)->id),
		viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	

	ImGui::SetCursorPos(ImVec2{10,10});
	if (ImGui::RadioButton("Move (W)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::TRANSLATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate (E)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::ROTATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale (R)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::SCALE; };
	ImGui::Text("Number of DrawCalls: %d " , m_renderer->NumberDraw);
	ImGui::Text("Number of Batches: %d ", m_renderer->NumberBatches);

	
	//IMGUIZMO STUFF STARTS HERE
	ImVec2 screen_pos = ImGui::GetMousePos();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	if (gizmoMode == ImGuizmo::OPERATION::TRANSLATE) { SetImGuizmoOperation(ImGuizmo::OPERATION::TRANSLATE); }
	if (gizmoMode == ImGuizmo::OPERATION::ROTATE) { SetImGuizmoOperation(ImGuizmo::OPERATION::ROTATE); }
	if (gizmoMode == ImGuizmo::OPERATION::SCALE) { SetImGuizmoOperation(ImGuizmo::OPERATION::SCALE); }

	if (InputManager::IsMouseDown(MOUSECODE::ButtonLeft) && m_ViewportFocused || InputManager::IsMouseDown(MOUSECODE::ButtonLeft)&& m_ViewportHovered)
	{
		

		if (IsGizmoValid())
		{
		
		}
		else
		{
			float width = static_cast<float>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
			float height = static_cast<float>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
			float region_x = screen_pos.x-pos.x;
			float region_y = height - ((pos.y - screen_pos.y)*-1) ;
			region_y = region_y - 50.0f; //remove Offset from menubar
			
			if (region_x > 0 && region_x < width && region_y > 0 && region_y < height)
			{		
				int id = (int)m_renderer->GetEntityID(region_x, region_y);
				if(id > 0)
				{
					Entity m_entity = *SceneManager::getEntity(id).get();
				
					if (m_entity)
					{
						SceneView::setSelectEntity(m_entity);
					}
				}
				else
				{
					SceneView::clearSelection();
				}
			}
			else
			{
				FRACTURE_INFO("Out of Bounds");
			}
		}	
	}		

	if (SceneView::SelectedEntity())
	{

		if (ComponentManager::HasComponent<TransformComponent>(SceneView::SelectedEntity().Id))
		{
			

			std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(SceneView::SelectedEntity().Id);
			std::shared_ptr<RelationShipComponent> relationship = ComponentManager::GetComponent<RelationShipComponent>(SceneView::SelectedEntity().Id);
			if (transform)
			{
				float rw = (float)ImGui::GetWindowWidth();
				float rh = (float)ImGui::GetWindowHeight();


				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);

				m_camera->setProjection(m_ViewportSize.x, m_ViewportSize.y);
				glm::mat4 viewMatrix = m_camera->getViewMatrix();
				glm::mat4 projectionMatrix = m_camera->getProjectionMatrix();
				glm::mat4 transformMatrix = transform->GetLocalTranform();		
			

				ImGuizmo::MODE mode = currentImGuizmoMode;
				if (currentImGuizmoOperation == ImGuizmo::OPERATION::SCALE && mode != ImGuizmo::MODE::LOCAL)
					mode = ImGuizmo::MODE::LOCAL;				

				ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
					currentImGuizmoOperation, mode, glm::value_ptr(transformMatrix)
				);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 scale; //= transform->Scale();
					glm::vec3 rotation; //= transform->Rotation();
					glm::vec3 position; //= transform->Position();
		
					Math::DecomposeTransform(transformMatrix, position, rotation, scale);
					
					glm::vec3 deltaRotation = rotation - transform->Rotation();
					transform->setScale(scale);
					transform->setRotation(deltaRotation += transform->Rotation());
					transform->setPosition(position);

				}
			}
		}

		if (ComponentManager::HasComponent<EditorNode>(SceneView::SelectedEntity().Id))
		{
			std::shared_ptr<EditorNode> node = ComponentManager::GetComponent<EditorNode>(SceneView::SelectedEntity().Id);
			if (node)
			{
				float rw = (float)ImGui::GetWindowWidth();
				float rh = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
				m_camera->setProjection((int)viewportPanelSize.x, (int)viewportPanelSize.y);
				glm::mat4 viewMatrix = m_camera->getViewMatrix();
				glm::mat4 projectionMatrix = m_camera->getProjectionMatrix();
				glm::mat4 transformMatrix = node->GetWorldTransform();
			

				ImGuizmo::MODE mode = currentImGuizmoMode;
				if (currentImGuizmoOperation == ImGuizmo::OPERATION::SCALE && mode != ImGuizmo::MODE::LOCAL)
					mode = ImGuizmo::MODE::LOCAL;

				ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
					currentImGuizmoOperation, mode, glm::value_ptr(transformMatrix)
				);

				if (ImGuizmo::IsUsing())
				{					

					glm::vec3 scale;
					glm::vec3 rotation;
					glm::vec3 position;

					Math::DecomposeTransform(transformMatrix, position, rotation, scale);
					
					glm::vec3 deltaRotation = rotation - node->GetRotation();
					node->SetScale(scale);
					node->SetRotation(deltaRotation += node->GetRotation());
					node->SetPosition(position);

				}
			}
		}
	}

    ImGui::PopStyleVar();
    ImGui::EndChild();

}

void Fracture::ViewPanel::onUpdate(float dt)
{
	ProfilerTimer timer("ViewPanel update");
	
	
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(m_renderer->SceneRenderTarget->Width != m_ViewportSize.x || m_renderer->SceneRenderTarget->Height != m_ViewportSize.y))
	{
		m_renderer->SceneRenderTarget->Resize(m_ViewportSize.x, m_ViewportSize.y);		
		m_renderer->m_PickingPass->Resize(m_ViewportSize.x, m_ViewportSize.y);
	}

	if(m_ViewportFocused && m_ViewportHovered && m_camera)
	{ 
		if (InputManager::IsMouseDown(MOUSECODE::ButtonRight))
		{	
			const glm::vec2& mouse{ InputManager::GetMousePosition().x,InputManager::GetMousePosition().y };
			glm::vec2 delta = (mouse - m_InitialMousePosition);
			m_InitialMousePosition = mouse;



			//SDL_WarpMouseInWindow(m_GameWindow->Context(), m_GameWindow->Width / 2, m_GameWindow->Height / 2);
			m_camera->InputMouse(mouse.x, mouse.y, dt);
			if (InputManager::IsKeyDown(KeyCode::W))
			{
				m_camera->Move(Camera_Movement::FORWARD, dt);
			}
			if (InputManager::IsKeyDown(KeyCode::S))
			{
				m_camera->Move(Camera_Movement::BACKWARD, dt);
			}
			if (InputManager::IsKeyDown(KeyCode::A))
			{
				m_camera->Move(Camera_Movement::LEFT, dt);
			}
			if (InputManager::IsKeyDown(KeyCode::D))
			{
				m_camera->Move(Camera_Movement::RIGHT, dt);
			}
			if (InputManager::IsKeyDown(KeyCode::E))
			{
				m_camera->Move(Camera_Movement::UP, dt);
			}
			if (InputManager::IsKeyDown(KeyCode::Q))
			{
				m_camera->Move(Camera_Movement::DOWN, dt);
			}

			m_camera->onUpdate(dt);
		}			
		if (InputManager::IsKeyDown(KeyCode::W))
		{
			gizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (InputManager::IsKeyDown(KeyCode::E))
		{
			gizmoMode = ImGuizmo::OPERATION::SCALE;
		}
		if (InputManager::IsKeyDown(KeyCode::R))
		{
			gizmoMode = ImGuizmo::OPERATION::ROTATE;
		}
	}
}

void Fracture::ViewPanel::SetImGuizmoOperation(ImGuizmo::OPERATION operation)
{
	currentImGuizmoOperation = operation;
}

ImGuizmo::OPERATION Fracture::ViewPanel::GetImGuizmoOperation() const
{
	return currentImGuizmoOperation;
}

void Fracture::ViewPanel::SetImGuizmoMode(ImGuizmo::MODE mode)
{
	currentImGuizmoMode = mode;
}

ImGuizmo::MODE Fracture::ViewPanel::GetImGuizmoMode() const
{
	return currentImGuizmoMode;
}

bool Fracture::ViewPanel::IsGizmoValid() const
{
	return ImGuizmo::IsOver() || ImGuizmo::IsUsing();
}
