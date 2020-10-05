#pragma once
#ifndef VIEWPANEL_H
#define VIEWPANEL_H

#include "Panel.h"

namespace Fracture
{
	class Game;

	class ViewPanel : public Panel
	{

	public:
		ViewPanel(std::string name);
		~ViewPanel();

		void setGame(Game* game);
		void render() override;

	private:
		Game* m_game;

	};

}

#endif