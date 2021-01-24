#pragma once
#ifndef CLEARFRAME_H
#define CLEARFRAME_H

#include "../FrameNode.h"

namespace Fracture
{

	class ClearFrame : public FrameNode
	{

	public:
		ClearFrame(std::string Name);

		void execute(Renderer& renderer) override;

	private:

		std::shared_ptr<FrameResource> resource;
		
	};


}

#endif