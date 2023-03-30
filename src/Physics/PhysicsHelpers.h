#pragma once

#include "physx/PxPhysicsAPI.h"


namespace Fracture
{
	enum class CookingResult
	{
		Success,
		ZeroAreaTestFailed,
		PolygonLimitReached,
		LargeTriangle,
		Failure
	};

	enum class ForceMode : uint8_t
	{
		Force = 0,
		Impulse,
		VelocityChange,
		Acceleration
	};



	namespace PhysicsHelpers {

		


		physx::PxTransform ToPhysXTransform(const glm::vec3& translation, const glm::vec3& rotation);

		physx::PxTransform ToPhysXTransform(const glm::mat4& transform);
		physx::PxMat44 ToPhysXMatrix(const glm::mat4& matrix);
		const physx::PxVec3& ToPhysXVector(const glm::vec3& vector);
		const physx::PxVec4& ToPhysXVector(const glm::vec4& vector);
		physx::PxQuat ToPhysXQuat(const glm::quat& quat);

		glm::mat4 FromPhysXTransform(const physx::PxTransform& transform);
		glm::mat4 FromPhysXMatrix(const physx::PxMat44& matrix);
		const glm::vec3& FromPhysXVector(const physx::PxVec3& vector);
		const glm::vec4& FromPhysXVector(const physx::PxVec4& vector);
		glm::quat FromPhysXQuat(const physx::PxQuat& quat);

		CookingResult FromPhysXCookingResult(physx::PxConvexMeshCookingResult::Enum cookingResult);
		CookingResult FromPhysXCookingResult(physx::PxTriangleMeshCookingResult::Enum cookingResult);
	}




}