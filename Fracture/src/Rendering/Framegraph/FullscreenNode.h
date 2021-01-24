#pragma once
#ifndef FULLSCREENNODE_H
#define FULLSCREENNODE_H

#include "FrameNode.h"

namespace Fracture
{

	class FullScreenNode :public FrameNode
	{

	public:
		FullScreenNode(std::string name);

		virtual void execute(Renderer& renderer);		
		// screen quad VAO
		unsigned int quadVAO, quadVBO;
	private:
		

	};


}

#endif