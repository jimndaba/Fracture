#include "ViewPanel.h"
#include "Rendering/RenderTarget.h"
#include "SceneviewPanel.h"
#include "../Editor.h"

#include <glm/gtx/matrix_decompose.hpp>
#include "../EditorFrameGraph.h"


int Fracture::ViewPanel::gizmoMode;


Fracture::ViewPanel::ViewPanel(std::string name,SceneView& scenegraph):Panel(name), m_scenegraph(scenegraph)
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

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { viewportPanelSize.x ,  viewportPanelSize.y };
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();
	
	ImGui::Image(reinterpret_cast<void*>(Editor::m_graph->GetOutput()->outputColor->GetColorTexture(0)->id),
		viewportPanelSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	
	ImGui::SetCursorPos(ImVec2{10,10});
	if (ImGui::RadioButton("Move (W)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::TRANSLATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate (E)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::ROTATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale (R)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::SCALE; };
	ImGui::Text("Number of DrawCalls: %d " , m_renderer->NumberDraw);
	ImGui::Text("Number of Batches: %d ", m_renderer->NumberBatches);
	ImGui::Text("Camera Position: %f , %f , %f ", m_renderer->ActiveCamera()->getPosition().x, m_renderer->ActiveCamera()->getPosition().y, m_renderer->ActiveCamera()->getPosition().z);
	ImGui::Text("Camera Right: %f , %f , %f ", m_renderer->ActiveCamera()->Right().x, m_renderer->ActiveCamera()->Right().y, m_renderer->ActiveCamera()->Right().z);
	ImGui::Text("Camera Up: %f , %f , %f ", m_renderer->ActiveCamera()->Up().x, m_renderer->ActiveCamera()->Up().y, m_renderer->ActiveCamera()->Up().z);
	ImGui::Text("Camera Front: %f , %f , %f ", m_renderer->ActiveCamera()->Front().x, m_renderer->ActiveCamera()->Front().y, m_renderer->ActiveCamera()->Front().z);
	ImGui::Text("Camera Yaw: %f ", m_renderer->ActiveCamera()->GetYaw());
	ImGui::Text("Camera Pitch: %f ", m_renderer->ActiveCamera()->GetPitch());
	ImGui::Text("Camera Roll: %f ", m_renderer->ActiveCamera()->GetRoll());

	
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
				int id = (int)m_renderer->GetEntityID((int)region_x,(int)region_y);
				if(id > 0)
				{
					const auto & m_entity = SceneManager::getEntity(id);
				
					if (m_entity)
					{
						m_scenegraph.setSelectEntity(m_entity);
					}
				}
				else
				{
					m_scenegraph.clearSelection();
				}
			}
			else
			{
				FRACTURE_INFO("Out of Bounds");
			}
		}	
	}		

	if (m_scenegraph.SelectedEntity())
	{
		std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(m_scenegraph.SelectedEntity()->Id);

		if (transform)
		{		

			//std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(m_scenegraph.SelectedEntity()->Id);
			std::shared_ptr<RelationShipComponent> relationship = ComponentManager::GetComponent<RelationShipComponent>(m_scenegraph.SelectedEntity()->Id);
			
			float rw = (float)ImGui::GetWindowWidth();
			float rh = (float)ImGui::GetWindowHeight();


			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);

			//m_camera->setProjection(m_ViewportSize.x, m_ViewportSize.y);
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

			if (ComponentManager::HasComponent<RenderComponent>(m_scenegraph.SelectedEntity()->Id))
			{
				auto& rendercomp = ComponentManager::GetComponent<RenderComponent>(m_scenegraph.SelectedEntity()->Id);
				std::vector<std::shared_ptr<Mesh>> meshes = rendercomp->GetModel()->GetMeshes();
				for (const auto& mesh : meshes)
				{
					m_renderer->PushOutlineCommand(m_scenegraph.SelectedEntity()->Id, mesh,transform->GetWorldTransform());
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
	Mouse m_mouse = InputManager::GetMouse();
	
	
	if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
		(m_renderer->SceneRenderTarget->Width != m_ViewportSize.x || m_renderer->SceneRenderTarget->Height != m_ViewportSize.y))
	{		
		m_renderer->SceneRenderTarget->Resize(m_ViewportSize.x, m_ViewportSize.y);
		m_renderer->setViewport(m_ViewportSize.x, m_ViewportSize.y);	
		Editor::m_graph->Resize(m_ViewportSize.x, m_ViewportSize.y);
		//TestGraph::Resize(m_ViewportSize.x, m_ViewportSize.y);	
	}

	if(m_ViewportHovered && m_camera)
	{ 
		
		const glm::vec2& mouse{ m_mouse.GetPosition().x,m_mouse.GetPosition().y };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.03f;
		m_InitialMousePosition = mouse;
		
		if (m_mouse.IsButtonDown(MOUSECODE::ButtonRight))
		{	
			m_camera->InputMouse(delta.x, delta.y, dt);

			//SDL_WarpMouseInWindow(m_GameWindow->Context(), m_GameWindow->Width / 2, m_GameWindow->Height / 2);
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

		

		if (InputManager::IsKeyDown(KeyCode::Q))
		{
			gizmoMode = ImGuizmo::OPERATION::TRANSLATE;
		}
		if (InputManager::IsKeyDown(KeyCode::W))
		{
			gizmoMode = ImGuizmo::OPERATION::SCALE;
		}
		if (InputManager::IsKeyDown(KeyCode::E))
		{
			gizmoMode = ImGuizmo::OPERATION::ROTATE;
		}
		if (InputManager::IsKeyDown(KeyCode::D) &&  m_scenegraph.m_selection)
		{
			if (InputManager::IsKeyDown(KeyCode::LeftControl) || InputManager::IsKeyDown(KeyCode::RightControl))
			{
				m_scenegraph.DuplicateSelection(m_scenegraph.m_selection);
			}			
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
