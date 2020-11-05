#include "ViewPanel.h"
#include "Rendering/RenderTarget.h"
#include "SceneviewPanel.h"
#include "../Editor.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <Component\EditorNodeComponent.h>

Fracture::ViewPanel::ViewPanel(std::string name):Panel(name)
{
}

Fracture::ViewPanel::~ViewPanel()
{
}


void Fracture::ViewPanel::init()
{
	m_camera = ComponentManager::GetComponent<CameraControllerComponent>(Editor::ActiveScene()->MainCamera()->Id);
	m_camera->onStart();
}

void Fracture::ViewPanel::setRenderer(Renderer* renderer)
{
    m_renderer = renderer;
}

void Fracture::ViewPanel::render()
{
	ProfilerTimer timer("viewPanel Render");
	static int gizmoMode = 0;
	if (ImGui::RadioButton("Translate (W)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::TRANSLATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate (E)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::ROTATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale (R)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::SCALE; };

	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };
	ImGuizmo::SetOrthographic(false);

	

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::BeginChild("SceneView");	

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail(); 
	ImVec2 viewPosition = { ImGui::GetWindowPos().x ,ImGui::GetWindowPos().y };
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y};
	
    ImGui::Image(reinterpret_cast<void*>(m_renderer->SceneRenderTarget->GetColorTexture(0)->id),
		viewportPanelSize, ImVec2{0, 1}, ImVec2{ 1, 0 });
	
	
	ImVec2 screen_pos = ImGui::GetMousePos();
	ImVec2 pos = ImGui::GetCursorScreenPos();

	if (gizmoMode == ImGuizmo::OPERATION::TRANSLATE) {SetImGuizmoOperation(ImGuizmo::OPERATION::TRANSLATE); }
	if (gizmoMode == ImGuizmo::OPERATION::ROTATE) { SetImGuizmoOperation(ImGuizmo::OPERATION::ROTATE); }
	if (gizmoMode == ImGuizmo::OPERATION::SCALE) { SetImGuizmoOperation(ImGuizmo::OPERATION::SCALE); }


	if (InputManager::IsMouseDown(MOUSECODE::ButtonLeft) && m_ViewportFocused)
	{
		if (IsGizmoValid())
		{
		
		}
		else
		{
			RayHit hit;
			float width = static_cast<float>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
			float height = static_cast<float>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);
			float region_x = screen_pos.x - pos.x;
			float region_y = pos.y - screen_pos.y;
			Ray ray = m_camera->ScreenPointToRay(glm::vec2(region_x, region_y), width, height);
			//m_renderer->DrawDebugLineRetained(ray.GetOrigin(), ray.GetEndPoint(1000),glm::vec4(1.0f,0.0f,0.0f,1.0f));
			if (PhysicsManager::RayCast(ray, hit))
			{
				FRACTURE_INFO("Ray Hit");
			}
		}	
	}
		

	if (SceneView::SelectedEntity())
	{

		if (ComponentManager::HasComponent<TransformComponent>(SceneView::SelectedEntity().Id))
		{
			

			std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(SceneView::SelectedEntity().Id);
			if (transform)
			{
				float rw = (float)ImGui::GetWindowWidth();
				float rh = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetDrawlist();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);

				glm::mat4 viewMatrix = m_camera->getViewMatrix();
				glm::mat4 projectionMatrix = m_camera->getProjectionMatrix(viewportPanelSize.x, viewportPanelSize.y);
				glm::mat4 transformMatrix = transform->GetWorldTransform();
				//transformMatrix = transformMatrix.tras

				ImGuizmo::MODE mode = currentImGuizmoMode;
				if (currentImGuizmoOperation == ImGuizmo::OPERATION::SCALE && mode != ImGuizmo::MODE::LOCAL)
					mode = ImGuizmo::MODE::LOCAL;

				ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
					currentImGuizmoOperation, mode, glm::value_ptr(transformMatrix)
				);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 scale = transform->Scale;
					glm::quat rotation = glm::quat(transform->Rotation);
					glm::vec3 position = transform->Position;
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(transformMatrix, scale, rotation, position, skew, perspective);

					//rotation = glm::conjugate(rotation);
					transform->Scale = scale;
					transform->Rotation = glm::eulerAngles(rotation);
					transform->Position = position;

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

				glm::mat4 viewMatrix = m_camera->getViewMatrix();
				glm::mat4 projectionMatrix = m_camera->getProjectionMatrix(viewportPanelSize.x, viewportPanelSize.y);
				glm::mat4 transformMatrix = node->GetWorldTransform();
				//transformMatrix = transformMatrix.tras

				ImGuizmo::MODE mode = currentImGuizmoMode;
				if (currentImGuizmoOperation == ImGuizmo::OPERATION::SCALE && mode != ImGuizmo::MODE::LOCAL)
					mode = ImGuizmo::MODE::LOCAL;

				ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
					currentImGuizmoOperation, mode, glm::value_ptr(transformMatrix)
				);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 scale = node->GetScale();
					glm::quat rotation = glm::quat(node->GetRotation());
					glm::vec3 position = node->GetPosition();
					glm::vec3 skew;
					glm::vec4 perspective;
					glm::decompose(transformMatrix, scale, rotation, position, skew, perspective);
					//rotation = glm::conjugate(rotation);
					node->SetScale(scale);
					node->SetRotation(glm::eulerAngles(rotation));
					node->SetPosition(position);

				}
			}
		}
	}

	
	//ImGuizmo::DrawGrid(&m_camera->getViewMatrix()[0][0], &m_camera->getProjectionMatrix(m_ViewportSize.x, m_ViewportSize.y)[0][0], identityMatrix, 100.f);

    ImGui::PopStyleVar();
    ImGui::EndChild();

}

void Fracture::ViewPanel::onUpdate(float dt)
{
	ProfilerTimer timer("ViewPanel update");
	
	
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(m_renderer->SceneRenderTarget->Width != m_ViewportSize.x || m_renderer->SceneRenderTarget->Height != m_ViewportSize.y))
	{
		m_renderer->SceneRenderTarget->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
		m_renderer->setViewport((int)m_ViewportSize.x, (int)m_ViewportSize.y);
	}

	if(m_ViewportFocused && m_camera)
	{ 
		if (InputManager::IsMouseDown(MOUSECODE::ButtonRight))
		{
			float mouseX = InputManager::GetMousePosition().x;
			float mouseY = InputManager::GetMousePosition().y;

			//SDL_WarpMouseInWindow(m_GameWindow->Context(), m_GameWindow->Width / 2, m_GameWindow->Height / 2);
			m_camera->InputMouse(mouseX, mouseY, dt);
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
