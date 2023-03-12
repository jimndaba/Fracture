#pragma once
#ifndef RENDERGRAPH_H
#define RENDERGRAPH_H


#include "Passes/ClearTargetPass.h"
#include "Passes/ForwardPass.h"
#include "Passes/PresentPass.h"

#include <queue>

namespace Fracture
{

	struct Rendergraph
	{
		Rendergraph();		
		std::unordered_map<std::string,Fracture::UUID> NameToPasses;
		std::unordered_map<std::string,std::shared_ptr<IPass>> Passes;
		std::vector<PassLink> PassLinks;
		std::deque<std::string> PassQueue;
	};




}


#endif