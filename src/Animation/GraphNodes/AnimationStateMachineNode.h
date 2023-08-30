#pragma once
#ifndef ANIMATIONSTATEMACHINENODE_H
#define ANIMATIONSTATEMACHINENODE_H

#include "IAnimationNode.h"

namespace Fracture
{

	struct AnimationStateMachineNode : IPoseNode
	{
		AnimationStateMachineNode();
		void Process(AnimationContext& context);

		Fracture::UUID Current_Node;
		Fracture::UUID Default_State;

		std::vector<Fracture::UUID> mStates;
		std::vector<Fracture::UUID> mTransitions;

		std::vector<std::unique_ptr<IAnimationNode>> mNodes;
		std::vector<NodeLink> Links;

	};





}

#endif