#pragma once
#ifndef ICAMERA_H
#define ICAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FrustumPlane.h"

namespace Fracture
{
	enum class Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	class ICamera
	{
	public:
		//view and projection Matricies
		virtual glm::mat4 getProjectionMatrix() = 0;
		virtual glm::mat4 getViewMatrix() = 0;
		virtual void setProjection(int width, int height) = 0;

		//movement
		virtual glm::vec3 getPosition() = 0;
		virtual void Move(Camera_Movement td, float dt) = 0;
		virtual void InputMouse(float xpos, float ypos, float dt, bool constrainPitch = true) = 0;
		virtual void onUpdate(float dt) = 0;

		void CalcFrustumPlanes();
		void GetFrustumPlanes(glm::vec4 planes[6]);
		bool IsPointInFrustum(const glm::vec3& point);
		bool IsSphereInFrustum(const glm::vec3& center, const float	radius);
		bool IsBoxInFrustum(const glm::vec3& min, const glm::vec3& max);

		glm::vec3 Right() { return right; }
		glm::vec3 Up() {return up;}
		float Near() { return Znear; }
		float Far() { return Zfar; }
		float GetApectRatio() { return aspect_ratio; }
		float GetFOV() { return fov; }
			
		//frustum points
		glm::vec3 farPts[4];
		glm::vec3 nearPts[4];
	protected:
		//Frsutum planes
		float fov, aspect_ratio, Znear, Zfar;
		glm::vec3 look;
		glm::vec3 up;
		glm::vec3 right;
		glm::vec3 front;
		glm::vec3 position;
		glm::mat4 V; //view matrix
		glm::mat4 P; //projection matrix
		int _width, _height;
		
		FrustumPlane planes[6];
	};
}

#endif