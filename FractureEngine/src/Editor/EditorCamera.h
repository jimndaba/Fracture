#pragma once

#include "Fracture.h"

namespace Fracture
{
	class EditorCamera:public ICamera,public IUPDATABLE
	{

	public:
		EditorCamera(glm::vec3 position = glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		~EditorCamera();
		glm::mat4 getProjectionMatrix(int width, int height) override;
		glm::mat4 getViewMatrix() override;
		glm::vec3 getPosition() override;
		void onUpdate(float dt);

		// Camera Attributes
		glm::vec3 Position = glm::vec3(0.0f, 5.0f, 15.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 LookTarget = glm::vec3(0, 0, 0);
		glm::vec3 WorldUp;
		// euler Angles
		float Yaw = 0.0f;
		float Pitch = -90.0f;
		float Roll = 0.0f;
		// camera options
		float mouseSpeed = 0.8f;
		float MovementSpeed = 10.0f;
		float MouseSensitivity = 0.3f;
		float Damping = 1.5f;
		float Zoom;
		float foV = 45.0f;
		float nearClip = 1.0f;
		float farClip = 100.0f;


		virtual void onStart();
		
		void Move(Camera_Movement td, float dt);
		void InputMouse(float xpos, float ypos, float dt, bool constrainPitch = true);
		void ZoomCamera(glm::vec2 zoom, float dt);

		void LookAt(glm::vec3 target);
		void Translate(glm::vec3 position);

		Ray ScreenPointToRay(glm::vec2 mousePosition, int viewWidth, int viewHeight);

		glm::vec3 m_TargetPosition = glm::vec3(0.0f, 5.0f, 15.0f);

	private:
		void UpdateCameraVectors();

		glm::mat4 m_viewMatrix;


		float m_TargetYaw = -90.0f;
		float m_TargetPitch = 0.0f;
		float targetZoom = 45.0f;

		float lastX = 1280.0f / 2.0f;
		float lastY = 720.0f / 2.0f;
	};
}

