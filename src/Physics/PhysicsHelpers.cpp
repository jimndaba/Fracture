#include "FracturePCH.h"
#include "PhysicsHelpers.h"
#include "Common/Math.h"

physx::PxTransform Fracture::PhysicsHelpers::ToPhysXTransform(const glm::vec3& translation, const glm::vec3& rotation)
{
    return physx::PxTransform(ToPhysXVector(translation), ToPhysXQuat(glm::quat(rotation)));
}


physx::PxTransform Fracture::PhysicsHelpers::ToPhysXTransform(const glm::mat4& transform)
{
    glm::vec3 translation, rotation, scale;
    Math::DecomposeTransform(transform, translation, rotation, scale);

    physx::PxQuat r = ToPhysXQuat(glm::quat(rotation));
    physx::PxVec3 p = ToPhysXVector(translation);
    return physx::PxTransform(p, r);

}

physx::PxMat44 Fracture::PhysicsHelpers::ToPhysXMatrix(const glm::mat4& matrix)
{
    return *(physx::PxMat44*) & matrix;
}

const physx::PxVec3& Fracture::PhysicsHelpers::ToPhysXVector(const glm::vec3& vector)
{
    return *(physx::PxVec3*) & vector;
}

const physx::PxVec4& Fracture::PhysicsHelpers::ToPhysXVector(const glm::vec4& vector)
{
    return *(physx::PxVec4*) & vector;
}

physx::PxQuat Fracture::PhysicsHelpers::ToPhysXQuat(const glm::quat& quat)
{
    return physx::PxQuat(quat.x, quat.y, quat.z, quat.w);
}

glm::mat4 Fracture::PhysicsHelpers::FromPhysXTransform(const physx::PxTransform& transform)
{
    glm::quat rotation = FromPhysXQuat(transform.q);
    glm::vec3 position = FromPhysXVector(transform.p);
    return glm::translate(glm::mat4(1.0F), position) * glm::toMat4(rotation);

}

glm::mat4 Fracture::PhysicsHelpers::FromPhysXMatrix(const physx::PxMat44& matrix)
{
    return *(glm::mat4*) & matrix;
}

const glm::vec3& Fracture::PhysicsHelpers::FromPhysXVector(const physx::PxVec3& vector)
{
    return *(glm::vec3*) & vector;
}

const glm::vec4& Fracture::PhysicsHelpers::FromPhysXVector(const physx::PxVec4& vector)
{
    return *(glm::vec4*) & vector;
}

glm::quat Fracture::PhysicsHelpers::FromPhysXQuat(const physx::PxQuat& quat)
{
    return *(glm::quat*) & quat;
}

Fracture::CookingResult Fracture::PhysicsHelpers::FromPhysXCookingResult(physx::PxConvexMeshCookingResult::Enum cookingResult)
{
    switch (cookingResult)
    {
    case physx::PxConvexMeshCookingResult::eSUCCESS: return CookingResult::Success;
    case physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED: return CookingResult::ZeroAreaTestFailed;
    case physx::PxConvexMeshCookingResult::ePOLYGONS_LIMIT_REACHED: return CookingResult::PolygonLimitReached;
    case physx::PxConvexMeshCookingResult::eFAILURE: return CookingResult::Failure;
    }

    return CookingResult::Failure;

}

Fracture::CookingResult Fracture::PhysicsHelpers::FromPhysXCookingResult(physx::PxTriangleMeshCookingResult::Enum cookingResult)
{
    switch (cookingResult)
    {
    case physx::PxTriangleMeshCookingResult::eSUCCESS: return CookingResult::Success;
    case physx::PxTriangleMeshCookingResult::eLARGE_TRIANGLE: return CookingResult::LargeTriangle;
    case physx::PxTriangleMeshCookingResult::eFAILURE: return CookingResult::Failure;
    }

    return CookingResult::Failure;

}