#pragma once
#ifndef TABPANEL_H
#define TABPANEL_H

#include "Panel.h"


namespace Fracture
{
	class TabbedElement;

	class TabbedPanel :public Panel
	{

	public:
		TabbedPanel(std::string name);
		~TabbedPanel();

		
		void render() override;


	private:

		std::shared_ptr<TabbedElement> m_tabElement;
	};

}

#endif