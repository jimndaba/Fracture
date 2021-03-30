#pragma once
#ifndef GAMEGRAPH_H
#define GAMEGRAPH_H

#include "Rendering/Framegraph/FrameGraph.h"

namespace Fracture {

	class GameFrameGraph : public FrameGraph
	{
	public:
		GameFrameGraph(Fracture::Renderer& renderer):FrameGraph(renderer)
		{

		}
		~GameFrameGraph() {};
	};
}

#endif