#include "Camera2D.h"
#include <glm\gtx\compatibility.hpp>

Fracture::Camera2D::Camera2D(glm::vec3  m_position, glm::vec3 m_up):ICamera(),IUPDATABLE()
{
    Yaw = -1.0f;
    Pitch = 0.0f;
    up = m_up;
    Znear = 0.5f;
    Zfar = 1000.0f;
    fov = 45.0f;
    position = m_position;
    UpdateCameraVectors();
    CalcFrustumPlanes();
}

glm::mat4 Fracture::Camera2D::getProjectionMatrix()
{
	return glm::ortho(0.0f, ScreenWidth, ScreenHeight,0.0f,-10.0f,10.0f);
}

glm::mat4 Fracture::Camera2D::getViewMatrix()
{
	return glm::lookAt(position, position + front, up);//-Position
}

void Fracture::Camera2D::setProjection(int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;
}

glm::vec3 Fracture::Camera2D::getPosition()
{
	return position;
}

void Fracture::Camera2D::onUpdate(float dt)
{
    //fov = glm::lerp(fov, targetZoom, dt * 3.0f);
    //position = glm::lerp(position, m_TargetPosition, dt * Damping);
    //Yaw = glm::lerp(Yaw, m_TargetYaw, dt * Damping);
    //Pitch = glm::lerp(Pitch, m_TargetPitch, dt * Damping);
    //UpdateCameraVectors();
    //if (changed)
    //{
    //    CalcFrustumPlanes();
    //    changed = false;
    //}
}

void Fracture::Camera2D::onStart()
{
}

void Fracture::Camera2D::Move(Camera_Movement td, float dt)
{
    //switch (td) {
    //case Camera_Movement::UP:
    //    m_TargetPosition -= glm::vec3(0.0f, MovementSpeed, 0.0f) * dt;
    //    break;
    //case Camera_Movement::DOWN:
    //    m_TargetPosition += glm::vec3(0.0f, MovementSpeed, 0.0f) * dt;
    //    break;
    //case Camera_Movement::LEFT:
    //    m_TargetPosition -= right * MovementSpeed * dt;
    //    break;
    //case Camera_Movement::RIGHT:
    //    m_TargetPosition += right * MovementSpeed * dt;
    //    break;
    //case Camera_Movement::FORWARD:
    //    m_TargetPosition += front * MovementSpeed * dt;
    //    break;
    //case Camera_Movement::BACKWARD:
    //    m_TargetPosition -= front * MovementSpeed * dt;
    //    break;
    //}
    //changed = true;
}

void Fracture::Camera2D::InputMouse(float xpos, float ypos, float dt, bool constrainPitch)
{
    //if (firstMouse)
    //{
    //    lastX = xpos;
    //    lastY = ypos;
    //    firstMouse = false;
    //}
    //
    //rX -= xpos * MouseSensitivity;// *xoffset;
    //rY += ypos * MouseSensitivity;// *yoffset;
    //
    //lastX = rX;
    //lastY = rY;
    //
    //m_TargetYaw = rX;
    //m_TargetPitch = rY;
    //m_TargetRoll = 0;
    //
    //changed = true;
}

void Fracture::Camera2D::ZoomCamera(glm::vec2 zoom, float dt)
{
}

void Fracture::Camera2D::Translate(glm::vec3 position)
{
    //m_TargetPosition = position;
}

void Fracture::Camera2D::UpdateCameraVectors()
{
    //glm::mat4 R = glm::mat4(1);
    //R = glm::rotate(R, Roll, glm::vec3(0, 0, 1));
    //R = glm::rotate(R, Yaw, glm::vec3(0, 1, 0));
    //R = glm::rotate(R, Pitch, glm::vec3(1, 0, 0));
    //front = glm::normalize(glm::vec3(R * glm::vec4(0, 0, 1, 0)));
    //up = glm::normalize(glm::vec3(R * glm::vec4(0, 1, 0, 0)));
    //right = glm::normalize(glm::cross(front, up));
}
