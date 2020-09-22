#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/compatibility.hpp"
#include <vector>
#include <math.h>

#include "Game/GameWindow.h"


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

	// Default camera values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	class Camera
	{
	private:
		glm::vec3 m_TargetPosition = glm::vec3(0.0f, 0.0f, 15.0f);
		float m_TargetYaw = -90.0f;
		float m_TargetPitch = 0.0f;
		float targetZoom = 45.0f;

		float lastX = 1280.0f / 2.0f;
		float lastY = 720.0f / 2.0f;
		bool firstMouse = true;

		void UpdateCameraVectors();

	public:

		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		~Camera() {};

		// Camera Attributes
		glm::vec3 Position = glm::vec3(0.0f, 0.0f, 15.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 WorldUp;
		// euler Angles
		float Yaw = 0.0f;
		float Pitch = -90.0f;
		float Roll = 0.0f;
		// camera options
		float mouseSpeed = 0.8f;
		float MovementSpeed = 5.0f;
		float MouseSensitivity = 0.3f;
		float Damping = 2.0f;
		float Zoom;
		float foV = 45.0f;
		float nearClip = 1.0f;
		float farClip = 100.0f;
			   
		glm::mat4 getViewMatrix();

		glm::mat4 getProjectionMatrix(GameWindow* m_window);

		void update(float dt);

		void Move(Camera_Movement td, float dt);

		void InputMouse(float xpos, float ypos, float dt, bool constrainPitch = true);

		void ZoomCamera(glm::vec2 zoom, float dt);


	};

}

#endif
