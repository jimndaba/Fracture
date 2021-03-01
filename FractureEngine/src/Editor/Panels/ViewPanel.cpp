#include "ViewPanel.h"
#include "Rendering/RenderTarget.h"
#include "SceneviewPanel.h"
#include "../Editor.h"

#include <glm/gtx/matrix_decompose.hpp>
#include "../EditorFrameGraph.h"
#include "ViewportMode.h"
#include "EditorViewModes.h"


int Fracture::ViewPanel::gizmoMode;
std::string Fracture::ViewPanel::m_currentViewMode;

Fracture::ViewPanel::ViewPanel(std::string name,SceneView& scenegraph, Renderer& renderer):Panel(name), m_scenegraph(scenegraph),
m_renderer(renderer),
m_RenderView(std::make_shared<RenderViewMode>()),
m_SSAOView(std::make_shared<SSAOViewMode>()),
m_DepthView(std::make_shared<DepthViewMode>())
{
	gizmoMode = 0;

	m_viewModes["RenderView"] = m_RenderView;
	m_viewModes["SSAO"] = m_SSAOView;
	m_viewModes["Depth"] = m_DepthView;
}

Fracture::ViewPanel::~ViewPanel()
{
}

void Fracture::ViewPanel::init()
{
	SetViewMode("RenderView");
}

void Fracture::ViewPanel::setRenderer(Renderer& renderer)
{
	m_camera = renderer.ActiveCamera();
}

void Fracture::ViewPanel::render()
{
	ProfilerTimer timer("viewPanel Render");

	float width = static_cast<float>(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x);
	float height = static_cast<float>(ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y);

	// Get offset
	//Vector2 offset = Vector2(ImGui::GetWindowPos()) + m_window_padding;
	
	static const float identityMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f };		

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::BeginChild("SceneView");

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	m_ViewportSize = { width ,  height };
	m_ViewportFocused = ImGui::IsWindowFocused();
	m_ViewportHovered = ImGui::IsWindowHovered();

	//Render Current View Mode
	m_Viewportmode->Render(Editor::m_graph, ImVec2{ m_ViewportSize.x,m_ViewportSize.y });
		
	ImGui::SetCursorPos(ImVec2{10,10});
	if (ImGui::RadioButton("Move (W)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::TRANSLATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate (E)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::ROTATE; };
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale (R)", &gizmoMode)) { gizmoMode = ImGuizmo::OPERATION::SCALE; };

	ImGui::Text("Number of DrawCalls: %d " , m_renderer.NumberDraw);
	ImGui::Text("Number of Batches: %d ", m_renderer.NumberBatches);
	ImGui::Text("Camera Position: %f , %f , %f ", m_renderer.ActiveCamera()->getPosition().x, m_renderer.ActiveCamera()->getPosition().y, m_renderer.ActiveCamera()->getPosition().z);
	ImGui::Text("Camera Right: %f , %f , %f ", m_renderer.ActiveCamera()->Right().x, m_renderer.ActiveCamera()->Right().y, m_renderer.ActiveCamera()->Right().z);
	ImGui::Text("Camera Up: %f , %f , %f ", m_renderer.ActiveCamera()->Up().x, m_renderer.ActiveCamera()->Up().y, m_renderer.ActiveCamera()->Up().z);
	ImGui::Text("Camera Front: %f , %f , %f ", m_renderer.ActiveCamera()->Front().x, m_renderer.ActiveCamera()->Front().y, m_renderer.ActiveCamera()->Front().z);
	ImGui::Text("Camera Yaw: %f ", m_renderer.ActiveCamera()->GetYaw());
	ImGui::Text("Camera Pitch: %f ", m_renderer.ActiveCamera()->GetPitch());
	ImGui::Text("Camera Roll: %f ", m_renderer.ActiveCamera()->GetRoll());

	ImGuiComboFlags flags = ImGuiComboFlags_None;
	//View mode 
	ImGui::PushItemWidth(100);
	ImGui::SetCursorPos(ImVec2{ ImGui::GetContentRegionAvail().x - 150, 10 });
	if (ImGui::BeginCombo("ViewMode", m_currentViewMode.c_str(), flags))
	{

		for (const auto& viewmode : m_viewModes)//
		{
			const bool is_selected = (m_currentViewMode == viewmode.first);
			if (ImGui::Selectable(viewmode.first.c_str(), is_selected))
			{
				m_currentViewMode = viewmode.first;
				SetViewMode(m_currentViewMode);
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}


		ImGui::EndCombo();
	}
	ImGui::PopItemWidth();

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
			
		
			
			float region_x = screen_pos.x-pos.x;
			float region_y = height - ((pos.y - screen_pos.y)*-1) ;
			region_y = region_y - 50.0f; //remove Offset from menubar
			
			if (region_x > 0 && region_x < width && region_y > 0 && region_y < height)
			{		
				UUID id = m_renderer.GetEntityID((int)region_x,(int)region_y);
				if(id)
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
		const auto& transform = ComponentManager::GetComponent<TransformComponent>(m_scenegraph.SelectedEntity()->GetId());

		if (transform)
		{		

			//std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(m_scenegraph.SelectedEntity()->GetId());
			std::shared_ptr<RelationShipComponent> relationship = ComponentManager::GetComponent<RelationShipComponent>(m_scenegraph.SelectedEntity()->GetId());
			
			float rw = (float)ImGui::GetWindowWidth();
			float rh = (float)ImGui::GetWindowHeight();


			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, rw, rh);
			//ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, m_ViewportSize.x, m_ViewportSize.y);
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

			if (ComponentManager::HasComponent<RenderComponent>(m_scenegraph.SelectedEntity()->GetId()))
			{
				auto& rendercomp = ComponentManager::GetComponent<RenderComponent>(m_scenegraph.SelectedEntity()->GetId());
				std::vector<std::shared_ptr<Mesh>> meshes = rendercomp->GetModel()->GetMeshes();
				for (const auto& mesh : meshes)
				{
					m_renderer.PushOutlineCommand(m_scenegraph.SelectedEntity()->GetId(), mesh,transform->GetWorldTransform());
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
		(m_renderer.SceneRenderTarget->Width != m_ViewportSize.x || m_renderer.SceneRenderTarget->Height != m_ViewportSize.y))
	{				
		//Editor::oEvent(new WindowResizeEvent((int)m_ViewportSize.x, (int)m_ViewportSize.y));
		m_renderer.setViewport((int)m_ViewportSize.x, (int)m_ViewportSize.y);
		Editor::m_graph->Resize((int)m_ViewportSize.x, (int)m_ViewportSize.y);	
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

void Fracture::ViewPanel::SetViewMode(const std::string& mode)
{
	m_Viewportmode = m_viewModes[mode];
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
