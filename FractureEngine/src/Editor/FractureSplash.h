#pragma once
#ifndef FRACTURESPLASH_H
#define FRACTURESPLASH_H

#include "Fracture.h"
#include "Editor.h"
#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "utils/FileDialogue.h"
#include <direct.h>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <iostream>

namespace Fracture
{

	class FractureSplash
	{

	public:
		FractureSplash(Editor* editor);
		~FractureSplash();

		bool Show();
		
		void Close();

		void onUpdate();
		void onBeginFrame();
		void onRender();
		void onEndFrame();

	private:

		Editor* m_editor;
		bool m_isShow;
		bool m_run;
		static bool p_open;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		std::unique_ptr<GameWindow> m_window;
		std::unique_ptr<AssetManager> m_AssetManger;
		std::shared_ptr<Logger> m_logger;

		void createNewProject(std::string filepath);
		void openProject(std::string filepath);
		void Style();
	};

}

#endif