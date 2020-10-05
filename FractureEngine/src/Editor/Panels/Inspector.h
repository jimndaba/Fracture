#pragma once
#ifndef INSPECTOR_H
#define INSPECTOR_H

#include "Panel.h"

namespace Fracture
{
	class TagComponentElement;

	class InspectorPanel: public Panel
	{
	public:
		InspectorPanel(std::string name);
		~InspectorPanel();

		void render() override;

	private:
		std::shared_ptr<TagComponentElement> m_tagcomponent;
	};

}

#endif