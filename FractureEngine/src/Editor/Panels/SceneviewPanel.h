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

		std::shared_ptr<Fracture::Entity> SelectedEntity();
		void setSelectEntity(std::shared_ptr<Entity> selection);
		void clearSelection();

		void DuplicateSelection(const std::shared_ptr<Entity>&);

		std::shared_ptr<Entity> m_selection;

	private:

		static std::shared_ptr<Scene> m_scene;
		

		void DrawEntityNode(const UUID& entity);
	};

}

#endif