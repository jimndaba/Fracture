#include "CameraControllerComponent.h"


Fracture::CameraControllerComponent::CameraControllerComponent(uint32_t id, glm::vec3 position, glm::vec3 up, float yaw, float pitch):Component(id,ComponentType::Camera)
{
    Yaw = -90.0f;
    Pitch = 0.0f;
    WorldUp = up;
    Right = glm::vec3(1.0f, 0.0f, 0.0f);
}

Fracture::CameraControllerComponent::~CameraControllerComponent()
{

}

void Fracture::CameraControllerComponent::onStart()
{
}

glm::mat4 Fracture::CameraControllerComponent::getViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Fracture::CameraControllerComponent::getProjectionMatrix(int width, int height)
{
    return glm::perspective(glm::radians(foV), float(width) / float(height), nearClip, farClip);
}

void Fracture::CameraControllerComponent::onUpdate(float dt)
{
    foV = glm::lerp(foV, targetZoom, dt * 3.0f);
    Position = glm::lerp(Position, m_TargetPosition, dt * Damping);
    Yaw = glm::lerp(Yaw, m_TargetYaw, dt * Damping * 5.0f);
    Pitch = glm::lerp(Pitch, m_TargetPitch, dt * Damping * 5.0f);
    UpdateCameraVectors();
}

void Fracture::CameraControllerComponent::Move(Camera_Movement td, float dt)
{
    switch (td) {
    case Camera_Movement::UP:
        m_TargetPosition -= glm::vec3(0.0f, MovementSpeed, 0.0f) * dt;
        break;
    case Camera_Movement::DOWN:
        m_TargetPosition += glm::vec3(0.0f, MovementSpeed, 0.0f) * dt;
        break;
    case Camera_Movement::LEFT:
        m_TargetPosition -= Right * MovementSpeed * dt;
        break;
    case Camera_Movement::RIGHT:
        m_TargetPosition += Right * MovementSpeed * dt;
        break;
    case Camera_Movement::FORWARD:
        m_TargetPosition += Front * MovementSpeed * dt;
        break;
    case Camera_Movement::BACKWARD:
        m_TargetPosition -= Front * MovementSpeed * dt;
        break;
    }
}

bool firstMouse = true;
void Fracture::CameraControllerComponent::InputMouse(float xpos, float ypos, float dt, bool constrainPitch)
{
    if (firstMouse)
    {        
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

   

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    if (m_TargetYaw == 0.0f) m_TargetYaw = 0.01f;
    if (m_TargetPitch == 0.0f) m_TargetPitch = 0.01f;

    m_TargetYaw += MouseSensitivity * xoffset;


    if (constrainPitch)
    {
        if (m_TargetPitch > 89.0f)
        {
            m_TargetPitch = 89.0f;
        }
        else if (m_TargetPitch < -89.0f)
        {
            m_TargetPitch = -89.0f;
        }
        else
        {
            m_TargetPitch += MouseSensitivity * yoffset;
        }
    }

}

void Fracture::CameraControllerComponent::ZoomCamera(glm::vec2 zoom, float dt)
{
}

void Fracture::CameraControllerComponent::UpdateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
    front.y = sin(glm::radians(Pitch));
    front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp)); // Normalize the vectors, because their length 
    Up = glm::normalize(glm::cross(Right, Front));
}
