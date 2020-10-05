#pragma once
#ifndef TAGCOMPONENTELEMENT_H
#define TAGCOMPONENTELEMENT_H

#include "Element.h"

namespace Fracture
{

	class TagComponentElement : public Element
	{

	public:
		TagComponentElement(std::string name);
		~TagComponentElement();

		virtual void begin();
		virtual void render();
		virtual void end();

	};


}

#endif