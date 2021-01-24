#pragma once
#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include "RenderGraph.h"

namespace Fracture
{
	class PickingPass;

	class TestGraph : public RenderGraph
	{
	public:
			TestGraph(Renderer& renderer, std::string name);

			static int PixelID(int x, int y);
			static void Resize(int width, int height);

	private:
			static std::unique_ptr<PickingPass> m_pickingPass;
	};

}

#endif