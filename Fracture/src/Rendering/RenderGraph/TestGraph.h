#pragma once
#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include "RenderGraph.h"

namespace Fracture
{

	class TestGraph : public RenderGraph
	{
		public:
			TestGraph(Renderer& renderer, std::string name);
	};

}

#endif