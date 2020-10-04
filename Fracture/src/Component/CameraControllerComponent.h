#pragma once
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H


#include "Component.h"
#include "IUpdatable.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtx/compatibility.hpp"
#include "Game/GameWindow.h"
#include <vector>
#include <math.h>

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

	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 5.0f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;


	class CameraControllerComponent :public Component,public IUPDATABLE
	{

	public:
		CameraControllerComponent(int id, glm::vec3 position = glm::vec3(0.0f, 0.0f, 15.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		~CameraControllerComponent();

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
		float MovementSpeed = 10.0f;
		float MouseSensitivity = 0.3f;
		float Damping = 2.0f;
		float Zoom;
		float foV = 45.0f;
		float nearClip = 1.0f;
		float farClip = 100.0f;

		glm::mat4 getViewMatrix();

		glm::mat4 getProjectionMatrix(int width,int height);

		virtual void onUpdate(float dt);
		virtual void onAttach();
		virtual void onDettach();

		void Move(Camera_Movement td, float dt);
		void InputMouse(float xpos, float ypos, float dt, bool constrainPitch = true);
		void ZoomCamera(glm::vec2 zoom, float dt);

	private:
		void UpdateCameraVectors();

		glm::vec3 m_TargetPosition = glm::vec3(0.0f, 0.0f, 15.0f);
		float m_TargetYaw = -90.0f;
		float m_TargetPitch = 0.0f;
		float targetZoom = 45.0f;

		float lastX = 1280.0f / 2.0f;
		float lastY = 720.0f / 2.0f;
		//bool firstMouse = true;

	};

}

#endif