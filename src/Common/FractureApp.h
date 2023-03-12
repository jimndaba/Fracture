#pragma once
#ifndef FRACTUREAPP_H
#define FRACTUREAPP_H

#include "AppWindow.h"


namespace Fracture
{

	class IFractureApp
	{
	public:
		IFractureApp();
		~IFractureApp();

		virtual bool Startup() = 0;
		virtual void Update() = 0;
		virtual void OnFrameStart() = 0;
		virtual void Shutdown() = 0;

		static bool ShouldWindowClose();

		static bool CreateAppWindow(const WindowCreationInfo* info);

		static std::unique_ptr<AppWindow> mWindow;
	};



}

#endif