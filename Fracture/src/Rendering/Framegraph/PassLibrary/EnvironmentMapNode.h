#pragma once
#ifndef ENVIRONMENTMAPNODE_H
#define ENVIRONMENTMAPNODE_H

#include "../FrameNode.h"

namespace Fracture
{

	class EnvironmentMapNode : public FrameNode
	{

	public:
		EnvironmentMapNode(std::string Name);

		void execute(Renderer& renderer) override;

	private:

		std::shared_ptr<FrameResource> resource;

	};


}

#endif