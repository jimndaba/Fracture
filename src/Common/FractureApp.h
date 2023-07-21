#pragma once
#ifndef FRACTUREAPP_H
#define FRACTUREAPP_H

#include "AppWindow.h"


namespace Fracture
{
	class SceneRenderer;

	class IFractureApp
	{
	public:
		IFractureApp();
		~IFractureApp();

		virtual bool Startup(AppWindow* window = nullptr) = 0;
		virtual void Update() = 0;
		virtual void OnFrameStart(SceneRenderer* renderer) = 0;
		virtual void Shutdown() = 0;

		bool ShouldWindowClose(Fracture::AppWindow* window);

		static std::unique_ptr<AppWindow> CreateAppWindow(const WindowCreationInfo* info);

		
	};



}

#endif