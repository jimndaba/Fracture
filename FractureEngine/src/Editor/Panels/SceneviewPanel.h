#pragma once
#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include "Panel.h"

namespace Fracture
{

	class SceneView : public Panel
	{

	public:
		SceneView(std::string name);
		~SceneView();

		void render() override;

	};

}

#endif