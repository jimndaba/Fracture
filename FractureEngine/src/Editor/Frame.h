#pragma once
#ifndef FRAME_H
#define FRAME_H

#include <memory>
#include <vector>
#include "SDL2/SDL.h"


namespace Fracture
{
	class Element;

	class Frame
	{

	public:
		Frame();
		~Frame();

		void begin(SDL_Window* window);
		void render();
		void end();

		void AddElement(std::shared_ptr<Element> element);
		void RemoveElement(std::shared_ptr<Element>  element);

		std::vector <std::shared_ptr<Element>> GetElements();

	private:

		std::vector<std::shared_ptr<Element>> m_elements;


	};


}

#endif