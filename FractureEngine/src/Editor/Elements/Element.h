#pragma once
#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include "imgui/imgui.h"

namespace Fracture
{

	class Element
	{

	public:
		Element(std::string name);
		~Element();

		virtual void begin()=0;
		virtual void render()=0;
		virtual void end()=0;

		std::string Name()
		{
			return m_name;
		}

	private:

		std::string m_name;

	};

}

#endif