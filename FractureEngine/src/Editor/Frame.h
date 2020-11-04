#pragma once
#ifndef FRAME_H
#define FRAME_H

#include <memory>
#include <vector>
#include "SDL2/SDL.h"
#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"

namespace Fracture
{
	class Panel;

	class Frame
	{

	public:
		Frame();
		~Frame();

		void begin(SDL_Window* window);
		void render();
		void end();

		void AddPanel(std::shared_ptr<Panel> panel);
		void RemovePanel(std::shared_ptr<Panel>  panel);
		void clearPanel();

		std::vector <std::shared_ptr<Panel>> GetElements();

		

	private:

		std::vector<std::shared_ptr<Panel>> m_panels;
		ImGuiWindowFlags window_flags;


	};


}

#endif