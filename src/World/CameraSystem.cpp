#include "FracturePCH.h"
#include "CameraSystem.h"
#include "Components.h"

Fracture::CameraSystem::CameraSystem()
{
}


void Fracture::CameraSystem::Update(float dt, Fracture::CameraComponent& component)
{
    component.FoV = glm::lerp(component.FoV, component.TargetFoV, dt * 3.0f);
    component.Position = glm::lerp(component.Position, component.TargetPosition, dt * component.Damping);
    component.Yaw = glm::lerp(component.Yaw, component.TargetYaw, dt * component.Damping * 5.0f);
    component.Pitch = glm::lerp(component.Pitch, component.TargetPitch, dt * component.Damping * 5.0f);
    UpdateCameraVectors(component);



    switch (component.ProjectionMode)
    {
        case CameraComponent::ProjectionType::Perspective:
        {
            component.ProjectMatrix = glm::perspective(
                glm::radians(component.FoV),
                float(component.Width) / float(component.Height),
                component.Near,
                component.Far);
            break;
        }
        case CameraComponent::ProjectionType::Orthographic:
        {
            component.ProjectMatrix = glm::ortho(0.0f, component.Width,component.Height,0.0f,component.Near,component.Far);               
            break;
        }
    }
   

    component.ViewMatrix = glm::lookAt(component.Position, component.Position + component.Front, component.Up);
}

void Fracture::CameraSystem::UpdateCameraVectors(Fracture::CameraComponent& component)
{
    glm::mat4 R = glm::mat4(1);
    R = glm::rotate(R, component.Roll, glm::vec3(0, 0, 1));
    R = glm::rotate(R, component.Yaw, glm::vec3(0, 1, 0));
    R = glm::rotate(R, component.Pitch, glm::vec3(1, 0, 0));
    component.Front = glm::normalize(glm::vec3(R * glm::vec4(0, 0, -1, 0)));
    component.Up = glm::normalize(glm::vec3(R * glm::vec4(0, 1, 0, 0)));
    component.Right = glm::normalize(glm::cross(component.Front, component.Up));
}

void Fracture::CameraSystem::InputMouse(Fracture::CameraComponent& component,float xpos, float ypos, float dt, bool constrainPitch)
{
    if (component.FirstMove)
    {
        lastX = xpos;
        lastY = ypos;
        component.FirstMove = false;
    }

    rX -= xpos * component.Sensitivity;
    rY -= ypos * component.Sensitivity;

    lastX = rX;
    lastY = rY;

    component.TargetYaw = rX;
    component.TargetPitch = rY;
    component.TargetRoll = 0;
}
