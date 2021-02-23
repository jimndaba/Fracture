#include "CameraControllerComponent.h"
#include "Physics/Ray.h"
#include "Logging/Logger.h"

Fracture::CameraControllerComponent::CameraControllerComponent(UUID id, glm::vec3 position, glm::vec3 up, float yaw, float pitch):Component(id)
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
    ///return m_viewMatrix;
    return glm::lookAt(Position, Position + Front, Up);//-Position

}

glm::mat4 Fracture::CameraControllerComponent::getProjectionMatrix()
{
    return glm::perspective(glm::radians(foV), float(_width) / float(_height), nearClip, farClip);
}

void Fracture::CameraControllerComponent::setProjection(int width, int height)
{
    _width = width;
    _height = height;
}

glm::vec3 Fracture::CameraControllerComponent::getPosition()
{
    return Position;
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
    static float lastX = xpos;
    static float lastY = ypos;
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
   

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

    lastX = xpos;
    lastY = ypos;

}

void Fracture::CameraControllerComponent::ZoomCamera(glm::vec2 zoom, float dt)
{
}

void Fracture::CameraControllerComponent::LookAt(glm::vec3 target)
{
    LookTarget = target;
}

void Fracture::CameraControllerComponent::Translate(glm::vec3 position)
{
    m_TargetPosition = position;
}

Fracture::Ray Fracture::CameraControllerComponent::ScreenPointToRay(glm::vec2 mousePosition, int viewWidth, int viewHeight)
{
    // these positions must be in range [-1, 1] (!!!), not [0, width] and [0, height]
    float mouseX = (mousePosition.x / viewWidth - 0.5f) * 2;//(2.0f * mousePosition.x) / viewWidth - 1.0f;
    float mouseY = (mousePosition.y / viewHeight - 0.5f) * 2;///1.0f - (2.0f * mousePosition.y) / viewHeight;
   
    glm::vec4 ray_clip = glm::vec4(mouseX, mouseY, -1.0, 1.0);
    setProjection(viewWidth, viewHeight);
    glm::vec4 toEyeCoords = glm::inverse(getProjectionMatrix()) * ray_clip;
    toEyeCoords = glm::vec4(toEyeCoords.x, toEyeCoords.y, -1.0, 0.0);

    glm::vec4 toWorldCoords = glm::inverse(getViewMatrix()) * toEyeCoords;

    glm::vec3 mouse_ray = glm::vec3(toWorldCoords.x, toWorldCoords.y, toWorldCoords.z);
    mouse_ray = glm::normalize(mouse_ray);

    return Ray(Position, mouse_ray);
}

void Fracture::CameraControllerComponent::Accept(ISceneProbe* visitor)
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
