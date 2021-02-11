#pragma once
#ifndef FREECAMERA_H
#define FREECAMERA_H


#include "Fracture.h"

namespace Fracture
{

	class FreeCamera :public ICamera, public IUPDATABLE
	{
	public:
		FreeCamera(glm::vec3 position = glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		~FreeCamera() = default;

		glm::mat4 getProjectionMatrix() override;
		glm::mat4 getViewMatrix() override;
		void setProjection(int width, int height) override;

		glm::vec3 getPosition() override;
		void onUpdate(float dt);

		
		// camera options
		float MouseSpeed = 0.8f;
		float MovementSpeed = 10.0f;
		float MouseSensitivity = 0.3f;
		float Damping = 10.0f;
		float Zoom;

		virtual void onStart();

		void Move(Camera_Movement td, float dt);
		void InputMouse(float xpos, float ypos, float dt, bool constrainPitch = true);
		void ZoomCamera(glm::vec2 zoom, float dt);
		void Translate(glm::vec3 position);


	private:

		void UpdateCameraVectors();
		glm::vec3 m_TargetPosition = glm::vec3(0.0f, 5.0f, 15.0f);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float m_TargetYaw = -90.0f;
		float m_TargetPitch = 0.0f;
		float m_TargetRoll = 0.0f;
		float targetZoom = 45.0f;
		float lastX = 0;
		float lastY = 0;
		bool changed = false;
		float rX =0;
		float rY = 0;
		bool firstMouse = true;

	};



}

#endif