#pragma once
#ifndef VIEWPORTMODE_H
#define VIEWPORTMODE_H

#include <cstdint>
#include <memory>
namespace Fracture
{
	class FrameGraph;
	class ViewportMode
	{
	public:
		virtual void Render(const std::shared_ptr<FrameGraph>& graph, const ImVec2& size) = 0;
	};
}

#endif 
