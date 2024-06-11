#include "FracturePCH.h"
#include "CameraSystem.h"
#include "Components.h"
#include "SceneManager.h"
#include "Rendering/AABB.h"

Fracture::CameraSystem::CameraSystem()
{
}


void Fracture::CameraSystem::Update(float dt, Fracture::CameraComponent& component)
{
    const auto& transform = SceneManager::GetComponent<TransformComponent>(component.GetID());

    if (transform)
    {
        auto rotation = glm::eulerAngles(transform->Rotation);
        component.TargetYaw = rotation.y;
        component.TargetPitch = rotation.x;
        component.TargetRoll = rotation.z;
        component.TargetPosition = transform->Position;

        component.FoV = glm::lerp(component.FoV, component.TargetFoV, dt * component.ZoomSpeed);
        component.Position = glm::lerp(component.Position, component.TargetPosition, dt * component.Damping);        
        component.Yaw = glm::lerp(component.Yaw, component.TargetYaw, dt * component.Damping * component.RotationSpeed);
        component.Pitch = glm::lerp(component.Pitch, component.TargetPitch, dt * component.Damping * component.RotationSpeed);
        component.Roll = glm::lerp(component.Roll, component.TargetRoll, dt * component.Damping * component.RotationSpeed);        
    }
    else
    {
        component.FoV = glm::lerp(component.FoV, component.TargetFoV, dt * component.ZoomSpeed);
        component.Position = glm::lerp(component.Position, component.TargetPosition, dt * component.Damping);
        component.Yaw = glm::lerp(component.Yaw, component.TargetYaw, dt * component.Damping * component.RotationSpeed);
        component.Pitch = glm::lerp(component.Pitch, component.TargetPitch, dt * component.Damping * component.RotationSpeed);
        component.Roll = glm::lerp(component.Roll, component.Roll, dt * component.Damping * component.RotationSpeed);
    }
    UpdateCameraVectors(component);

   

    glm::vec3 cN = component.Position + component.Front * component.Near;
    glm::vec3 cF = component.Position + component.Front * component.Far;
    float aspect_ratio = component.AspectRatio;
    float Hnear = 2.0f * tan(component.FoV/ 2.0f) * component.Near;
    float Wnear = Hnear * aspect_ratio;
    float Hfar = 2.0f * tan(component.FoV / 2.0f) * component.Far;
    float Wfar = Hfar * aspect_ratio;
    float hHnear = Hnear / 2.0f;
    float hWnear = Wnear / 2.0f;
    float hHfar = Hfar / 2.0f;
    float hWfar = Wfar / 2.0f;

    glm::vec3 farPts[4];
    glm::vec3 nearPts[4];

    farPts[0] = cF +  component.Up * hHfar -  component.Right * hWfar;
    farPts[1] = cF -  component.Up * hHfar -  component.Right * hWfar;
    farPts[2] = cF -  component.Up * hHfar +  component.Right * hWfar;
    farPts[3] = cF +  component.Up * hHfar +  component.Right * hWfar;

    nearPts[0] = cN +  component.Up * hHnear -  component.Right * hWnear;
    nearPts[1] = cN -  component.Up * hHnear -  component.Right * hWnear;
    nearPts[2] = cN -  component.Up * hHnear +  component.Right * hWnear;
    nearPts[3] = cN +  component.Up * hHnear +  component.Right * hWnear;

     component.CameraFustrum.planes[0] = FrustumPlane::FromPoints(nearPts[3], nearPts[0], farPts[0]);
     component.CameraFustrum.planes[1] = FrustumPlane::FromPoints(nearPts[1], nearPts[2], farPts[2]);
     component.CameraFustrum.planes[2] = FrustumPlane::FromPoints(nearPts[0], nearPts[1], farPts[1]);
     component.CameraFustrum.planes[3] = FrustumPlane::FromPoints(nearPts[2], nearPts[3], farPts[2]);
     component.CameraFustrum.planes[4] = FrustumPlane::FromPoints(nearPts[0], nearPts[3], nearPts[2]);
     component.CameraFustrum.planes[5] = FrustumPlane::FromPoints(farPts[3], farPts[0], farPts[1]);



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

glm::vec3 Fracture::CameraSystem::GetVertexP(const glm::vec3& normal, const glm::vec3& min, const glm::vec3& max)
{
    glm::vec3 res = min;

    if (normal.x > 0)
        res.x += (max.x - min.x);

    if (normal.y > 0)
        res.y += (max.y - min.y);

    if (normal.z > 0)
        res.z += (max.z - min.z);

    return(res);
}

glm::vec3 Fracture::CameraSystem::GetVertexN(const glm::vec3& normal, const glm::vec3& min, const glm::vec3& max)
{
    glm::vec3 res = min;

    if (normal.x < 0)
        res.x += (max.x - min.x);

    if (normal.y < 0)
        res.y += (max.y - min.y);

    if (normal.z < 0)
        res.z += (max.z - min.z);

    return(res);
}

bool Fracture::CameraSystem::IsPointInFrustum(Fracture::CameraComponent& component, const glm::vec3& point)
{
    for (int i = 0; i < 6; i++)
    {
        if (component.CameraFustrum.planes[i].GetDistance(point) < 0)
            return false;
    }
    return true;
}

bool Fracture::CameraSystem::IsSphereInFrustum(Fracture::CameraComponent& component, const glm::vec3& center, const float radius)
{
    for (int i = 0; i < 6; i++)
    {
        float d = component.CameraFustrum.planes[i].GetDistance(center);
        if (d < -radius)
            return false;
    }
    return true;
}

bool Fracture::CameraSystem::IsBoxInFrustum(Fracture::CameraComponent& component, const AABB& aabb)
{
    return IsBoxInFrustum(component, aabb.min, aabb.max);
}

bool Fracture::CameraSystem::IsBoxInFrustum(Fracture::Frustum& frustum, const glm::vec3& min, const glm::vec3& max)
{
    bool result = true;
    for (int i = 0; i < 6; i++)
    {
        glm::vec3 normal = frustum.planes[i].normal;

        if (frustum.planes[i].GetDistance(GetVertexP(normal, min, max)) < 0)
        {
            result = false;
        }
        else if (frustum.planes[i].GetDistance(GetVertexN(normal, min, max)) < 0)
            result = true;
    }
    return result;
}

bool Fracture::CameraSystem::IsBoxInFrustum(Fracture::CameraComponent& component, const glm::vec3& min, const glm::vec3& max)
{
    return IsBoxInFrustum(component.CameraFustrum, min, max);
}
