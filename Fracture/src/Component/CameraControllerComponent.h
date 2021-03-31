#pragma once
#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H


#include "Component.h"
#include "IUpdatable.h"
#include "ICamera.h"


namespace Fracture
{
	

	const float cYAW = 0.5f;
	const float cPITCH = 0.0f;
	const float cSPEED = 5.0f;
	const float cSENSITIVITY = 0.1f;
	const float cZOOM = 45.0f;

	class Ray;


	class CameraControllerComponent :public Component,public IUPDATABLE,public ICamera
	{

	public:
		CameraControllerComponent(UUID id, glm::vec3 position = glm::vec3(0.0f, 5.0f, 15.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = cYAW, float pitch = cPITCH);
		~CameraControllerComponent();

		CameraControllerComponent(const CameraControllerComponent& component, UUID id) :Component(id)
		{
			Position = component.Position;
			Front = component.Front;
			Up = component.Up;
			foV = component.foV;
			WorldUp = component.WorldUp;
			Zoom = component.Zoom;
			m_viewMatrix = component.m_viewMatrix;

			Yaw = component.Yaw;
			Pitch = component.Pitch;
			Roll = component.Roll;
			nearClip = component.nearClip;
			farClip = component.farClip;


		}

		// Camera Attributes
		glm::vec3 Position = glm::vec3(0.0f, 5.0f, 15.0f);
		glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 LookTarget = glm::vec3(0,0,0);
		glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		// euler Angles
		float Yaw = 0.0f;
		float Pitch = 0.0f;
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

		glm::mat4 getViewMatrix() override;

		glm::mat4 getProjectionMatrix() override;
		void setProjection(int width, int height) override;

		glm::vec3 getPosition() override;

		virtual void onUpdate(float dt);

		void Move(Camera_Movement td, float dt);
		void InputMouse(float xpos, float ypos, float dt, bool constrainPitch = true);
		void ZoomCamera(glm::vec2 zoom, float dt);

		void LookAt(glm::vec3 target);
		void Translate(glm::vec3 position);

		Ray ScreenPointToRay(glm::vec2 mousePosition, int viewWidth,int viewHeight);

		glm::vec3 m_TargetPosition = glm::vec3(0.0f, 5.0f, 15.0f);
		void Accept(ISceneProbe* visitor) override;

		std::shared_ptr<CameraControllerComponent> clone(UUID id) const
		{
			return std::shared_ptr<CameraControllerComponent>(this->clone_impl(id));
		}


		json serialise(const std::shared_ptr<ComponentSerialiser>& visitor) override;

		bool IsActveCamera = false;

	private:

		virtual CameraControllerComponent* clone_impl(UUID id) const override
		{
			return new CameraControllerComponent(*this, id);
		}


		void UpdateCameraVectors();

		glm::mat4 m_viewMatrix;

		
	
		float m_TargetYaw = -1.0f;
		float m_TargetPitch = 0.0f;
		float targetZoom = 45.0f;

		float lastX = 1280.0f / 2.0f;
		float lastY = 720.0f / 2.0f;
		//bool firstMouse = true;

	};

}

#endif