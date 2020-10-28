#pragma once
#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "Panel.h"
#include "Fracture.h"

namespace Fracture
{
	class Scene;

	class SceneView : public Panel
	{

	public:
		SceneView(std::string name);
		~SceneView();

		void setScene(std::shared_ptr<Scene> scene);

		void render() override;

		static Entity SelectedEntity();

	private:

		std::shared_ptr<Scene> m_scene;
		static Entity m_selection;

		void DrawEntityNode(uint32_t entity);
	};

}

#endif