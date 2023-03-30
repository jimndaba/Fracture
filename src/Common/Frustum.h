#pragma once
#ifndef FRUSTUM_H
#define FRUSTUM_H

namespace Fracture
{
    struct CameraComponent;
    struct FrustumPlane
    {
        FrustumPlane() = default;
        FrustumPlane(const glm::vec3& p1, const glm::vec3& norm)
            : normal(glm::normalize(norm)),
            distance(glm::dot(normal, p1))
        {}

        float getSignedDistanceToPlan(const glm::vec3& point) const
        {
            return glm::dot(normal, point) - distance;
        }

        // unit vector
        glm::vec3 normal = { 0.f, 1.f, 0.f };
        // distance from origin to the nearest point in the plan
        float     distance = 0.f;
    };
    struct AABB;
    struct Frustum
    {
        Frustum();

        FrustumPlane topFace;
        FrustumPlane bottomFace;
        FrustumPlane rightFace;
        FrustumPlane leftFace;
        FrustumPlane farFace;
        FrustumPlane nearFace;

        static Frustum CreateFrustumFromCamera(const CameraComponent* cam);

    };


}

#endif