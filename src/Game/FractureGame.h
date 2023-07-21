#pragma once
#ifndef FRACTUREGAME_H
#define FRACTUREGAME_H

#include "Common/FractureApp.h"
#include "Rendering/SceneRenderer.h"

namespace Fracture
{
	class GraphicsDevice;
	class AssetManager;
	class Logger;
	class Input;
	class Eventbus;
	class SceneManager;
	class DebugRenderer;
	class ScriptManager;
	class AudioManager;
	class PhysicsManager;
	class AppWindow;


	class FractureGame : public IFractureApp
	{
		static bool GameRunning;
	public:
		FractureGame();

		bool Run(UUID entry_point, AppWindow* window = nullptr, SceneRenderer* renderer = nullptr);
		bool Startup(AppWindow* window = nullptr);

		bool CreateGameWindow(AppWindow* window = nullptr);

		void OnLoadContent();
		void Update();
		void OnFrameStart(SceneRenderer* renderer);
		void Shutdown();


		std::unique_ptr<Logger> logger;
		std::unique_ptr<Input> InputManager;
		std::unique_ptr<Eventbus> Dispatcher;
		std::unique_ptr<SceneManager> sceneManger;
		std::unique_ptr<DebugRenderer> mDebugRenderer;
		std::unique_ptr<ScriptManager> mScriptManager;
		std::unique_ptr<AudioManager> mAudioManager;		
		std::unique_ptr<AppWindow> mWindow;

	};



}

#endif