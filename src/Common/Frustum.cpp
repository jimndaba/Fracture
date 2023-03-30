#include "FracturePCH.h"
#include "Frustum.h"
#include "World/Components.h"

Fracture::Frustum::Frustum()
{
}

Fracture::Frustum Fracture::Frustum::CreateFrustumFromCamera(const CameraComponent* cam)
{
    /*
    Frustum     frustum;
    const float halfVSide = cam->Far * tanf(cam->FoV * .5f);
    const float halfHSide = halfVSide * cam->AspectRatio;
    const glm::vec3 frontMultFar = cam->Far * cam->Front;

    frustum.nearFace= { cam->Position + cam->Near * cam->Front,cam->Front};
    frustum.farFace = { cam->Position + frontMultFar, -cam->Front };
    frustum.rightFace = { cam->Position,glm::cross(cam->Up,frontMultFar + cam->Right * halfHSide) };
    frustum.leftFace = { cam->Position,
                            glm::cross(frontMultFar - cam->Right * halfHSide, cam->Up) };
    frustum.topFace = { cam->Position,
                            glm::cross(cam->Right, frontMultFar - cam->Up * halfVSide) };
    frustum.bottomFace = { cam->Position,
                            glm::cross(frontMultFar + cam->Up * halfVSide, cam->Right) };
    return frustum;
    */
}
