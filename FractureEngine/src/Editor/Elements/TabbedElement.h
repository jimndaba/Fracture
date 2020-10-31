#pragma once
#ifndef TABBEDELEMENT_H
#define TABBEDELEMENT_H

#include "Element.h"
#include "Fracture.h"

namespace Fracture
{

	class TabbedElement:public Element
	{

	public:
		TabbedElement(std::string name);
		~TabbedElement();

		virtual void begin();
		virtual void render();
		virtual void end();
		
		ImGuiTextBuffer     m_buffer;
		ImGuiTextFilter     m_filter;


	};

}

#endif