#pragma once
#ifndef FULLSCREENNODE_H
#define FULLSCREENNODE_H

#include "FrameNode.h"

namespace Fracture
{

	class FullScreenNode :public FrameNode
	{

	public:
		FullScreenNode(const std::string&);

		virtual void execute(Renderer& renderer);		
		// screen quad VAO
		unsigned int quadVAO, quadVBO;		
	private:
		

	};


}

#endif