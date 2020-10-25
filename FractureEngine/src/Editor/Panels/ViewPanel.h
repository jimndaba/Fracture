#pragma once
#ifndef VIEWPANEL_H
#define VIEWPANEL_H

#include "Panel.h"
#include "Fracture.h"

namespace Fracture
{
	
	class ViewPanel : public Panel
	{

	public:
		ViewPanel(std::string name);
		~ViewPanel();

		void init();
		void setRenderer(Renderer* renderer);
		void render() override;
		void onUpdate(float dt);

	private:
		Renderer* m_renderer;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		std::shared_ptr<CameraControllerComponent> m_camera;
	};

}

#endif