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
		static void setSelectEntity(Entity selection);

		static Entity m_selection;

	private:

		std::shared_ptr<Scene> m_scene;
		

		void DrawEntityNode(uint32_t entity);
	};

}

#endif