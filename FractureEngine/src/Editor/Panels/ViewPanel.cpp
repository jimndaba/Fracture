#include "ViewPanel.h"
#include "Rendering/RenderTarget.h"
#include "SceneviewPanel.h"
#include "../Editor.h"



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
	int gizmoMode = 1;
	ImGui::RadioButton("Translate (W)", &gizmoMode);
	ImGui::SameLine();
	ImGui::RadioButton("Rotate (E)", &gizmoMode);
	ImGui::SameLine();
	ImGui::RadioButton("Scale (R)", &gizmoMode);

	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };
	ImGuizmo::SetOrthographic(false);
	ImGuizmo::BeginFrame();
	

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::BeginChild("SceneView");
	

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };   
	ImVec2 viewPosition = { ImGui::GetWindowPos().x ,ImGui::GetWindowPos().y };
	ImGuizmo::SetRect(viewPosition.x, viewPosition.y, m_ViewportSize.x, m_ViewportSize.y);	

    ImGui::Image(reinterpret_cast<void*>(m_renderer->SceneRenderTarget->GetColorTexture(0)->id),
        ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{0, 1}, ImVec2{ 1, 0 });

	ImVec2 screen_pos = ImGui::GetMousePos();
	ImVec2 pos = ImGui::GetCursorScreenPos();


	if (InputManager::IsMouseDown(MOUSECODE::ButtonLeft) && m_ViewportFocused)
	{

		RayHit hit;

		float width = static_cast<float>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
		float height = static_cast<float>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

		float region_x = screen_pos.x - pos.x;
		float region_y = pos.y - screen_pos.y;

		Ray ray = m_camera->ScreenPointToRay(glm::vec2(region_x, region_y), width, height);

		m_renderer->DrawDebugLineRetained(ray.GetOrigin(), ray.GetEndPoint(1000));
		if (PhysicsManager::RayCast(ray, hit))
		{
			FRACTURE_INFO("Ray Hit");
		}

	}
		

	if (SceneView::SelectedEntity())
	{
		
		std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(SceneView::SelectedEntity().Id);

		if (transform)
		{
			glm::mat4 viewMatrix = m_camera->getViewMatrix();
			glm::mat4 projectionMatrix = m_camera->getProjectionMatrix(m_ViewportSize.x, m_ViewportSize.y);
			glm::mat4 transformMatrix = transform->GetWorldTransform();
			//transformMatrix = transformMatrix.tras

			ImGuizmo::MODE mode = currentImGuizmoMode;
			if (currentImGuizmoOperation == ImGuizmo::OPERATION::SCALE && mode != ImGuizmo::MODE::LOCAL)
				mode = ImGuizmo::MODE::LOCAL;

			ImGuizmo::Manipulate(
				&viewMatrix[0][0], &projectionMatrix[0][0],
				currentImGuizmoOperation, mode, &transformMatrix[0][0]
			);

			if (ImGuizmo::IsUsing())
			{

			}

		}

	}
	

	ImGuizmo::DrawGrid(&m_camera->getViewMatrix()[0][0], &m_camera->getProjectionMatrix(m_ViewportSize.x, m_ViewportSize.y)[0][0], identityMatrix, 100.f);


	
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
		Eventbus::Publish(new WindowResizeEvent((int)m_ViewportSize.x, (int)m_ViewportSize.y));
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
