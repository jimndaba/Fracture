#pragma once
#ifndef CAMERASYSTEM_H
#define CAMERASYSTEM_H


namespace Fracture
{
	struct CameraComponent;

	struct CameraSystem
	{

		CameraSystem();

		void Update(float dt, Fracture::CameraComponent& component);

		void UpdateCameraVectors(Fracture::CameraComponent& component);

		void InputMouse(Fracture::CameraComponent& component,float xpos, float ypos, float dt, bool constrainPitch);

	private:
		float lastX = 0;
		float lastY = 0;
		float rX = 0;
		float rY = 0;
	};





}


#endif