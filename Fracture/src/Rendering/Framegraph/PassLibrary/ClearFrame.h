#pragma once
#ifndef CLEARFRAME_H
#define CLEARFRAME_H

#include "../FrameNode.h"
#include "glm/glm.hpp"

namespace Fracture
{

	class ClearFrame : public FrameNode
	{

	public:
		ClearFrame(const std::string& Name);

		void execute(Renderer& renderer) override;
		json Accept(const std::shared_ptr<FrameNodeSerialiser>& visitor);

		glm::vec3 Color = glm::vec3(1.0f);

	private:

		std::shared_ptr<FrameResource> resource;
		
	};


}

#endif