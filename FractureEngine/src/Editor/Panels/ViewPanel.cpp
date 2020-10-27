#include "ViewPanel.h"
#include "Rendering/RenderTarget.h"
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

	
	
	if (ImGui::ImageButton((void*)AssetManager::getTexture("TranslateIcon")->id, ImVec2(32,32), ImVec2(0, 0), ImVec2(1, 1),1))
	{

	}
	ImGui::SameLine();
	if (ImGui::ImageButton((void*)AssetManager::getTexture("RotateIcon")->id, ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 1))
	{

	}
	ImGui::SameLine();
	if (ImGui::ImageButton((void*)AssetManager::getTexture("ScaleIcon")->id, ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), 1))
	{

	}
	

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::BeginChild("SceneView");
	

	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

    //m_renderer->setViewport(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);    
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };   



    ImGui::Image(reinterpret_cast<void*>(m_renderer->SceneRenderTarget->GetColorTexture(0)->id),
        ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{0, 1}, ImVec2{ 1, 0 });

	
    ImGui::PopStyleVar();
    ImGui::EndChild();

}

void Fracture::ViewPanel::onUpdate(float dt)
{
	

	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(m_renderer->SceneRenderTarget->Width != m_ViewportSize.x || m_renderer->SceneRenderTarget->Height != m_ViewportSize.y))
	{
		m_renderer->SceneRenderTarget->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);
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
