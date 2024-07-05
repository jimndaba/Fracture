#pragma once
#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H


namespace Fracture
{
	struct CameraComponent;
	struct AABB;
	struct Frustum;

	struct CameraSystem
	{

		CameraSystem();

		void Update(float dt, Fracture::CameraComponent& component);

		void UpdateCameraVectors(Fracture::CameraComponent& component);

		void InputMouse(Fracture::CameraComponent& component,float xpos, float ypos, float dt, bool constrainPitch);
		glm::vec3 GetVertexP(const glm::vec3& normal, const glm::vec3& min, const  glm::vec3& max);
		glm::vec3 GetVertexN(const glm::vec3& normal, const glm::vec3& min, const glm::vec3& max);
		bool IsPointInFrustum(Fracture::CameraComponent& component, const glm::vec3& point);
		bool IsSphereInFrustum(Fracture::CameraComponent& component, const glm::vec3& center, const float	radius);
		bool IsBoxInFrustum(Fracture::CameraComponent& component, const AABB& aabb);
		bool IsBoxInFrustum(Fracture::Frustum& frustum, const glm::vec3& min, const glm::vec3& max);
		bool IsBoxInFrustum(Fracture::CameraComponent& component, const glm::vec3& min, const glm::vec3& max);
		bool IntersectRayTriangle(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, glm::vec3& outIntersection);

		glm::vec3 ScreenToWorldRay(Fracture::CameraComponent& camera, float mouse_x,float mouse_y, float width, float height);

	private:
		float lastX = 0;
		float lastY = 0;
		float rX = 0;
		float rY = 0;
	};





}


#endif