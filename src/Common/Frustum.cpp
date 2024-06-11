#include "FracturePCH.h"
#include "Frustum.h"
#include "World/Components.h"

Fracture::Frustum::Frustum()
{
}

Fracture::Frustum Fracture::Frustum::CreateFrustumFromCamera(const CameraComponent* cam)
{
   
    Frustum     frustum;
    frustum.planes.resize(6);
    const float halfVSide = cam->Far * tanf(cam->FoV * .5f);
    const float halfHSide = halfVSide * cam->AspectRatio;
    const glm::vec3 frontMultFar = cam->Far * cam->Front;

    frustum.planes[0] = {cam->Position + cam->Near * cam->Front,cam->Front};
    frustum.planes[1] = { cam->Position + frontMultFar, -cam->Front };
    frustum.planes[2] = { cam->Position,glm::cross(cam->Up,frontMultFar + cam->Right * halfHSide) };
    frustum.planes[3] = { cam->Position,
                            glm::cross(frontMultFar - cam->Right * halfHSide, cam->Up) };
    frustum.planes[4] = { cam->Position,
                            glm::cross(cam->Right, frontMultFar - cam->Up * halfVSide) };
    frustum.planes[5] = { cam->Position,
                            glm::cross(frontMultFar + cam->Up * halfVSide, cam->Right) };
    return frustum;    
}

float Fracture::FrustumPlane::GetDistance(const glm::vec3& p)
{
    return glm::dot(normal, p) + distance;
}

Fracture::FrustumPlane Fracture::FrustumPlane::FromPoints(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
    FrustumPlane temp;
    glm::vec3 e1 = v2 - v1;
    glm::vec3 e2 = v3 - v1;
    temp.normal = glm::normalize(glm::cross(e1, e2));
    temp.distance = -glm::dot(temp.normal, v2);
    return temp;
}
