#pragma once
#ifndef ANIMATIONTRANSITIONNODE_H
#define ANIMATIONTRANSITIONNODE_H


#include "Animation/GraphNodes/IAnimationNode.h"
#include "Animation/GraphNodes/ValueNodes.h"

namespace Fracture
{


	struct AnimationTransitionNode : IPoseNode
	{
		enum class TransitionNodeState
		{
			None,
			BeginTransition,
			Transitioning,
			EndTransition
		};

		AnimationTransitionNode();
		void Process(AnimationContext& context);

		float TransitionOffset = 0.0f;
		float TransitionStartTime = 0.0f;
		float TransitionEndTime = 0.0f;
		float TransitionDuration = 0.0f;
		float TotalTransitionTime = 0.0f;

		float start_transition = 0.0f;
		float end_transition = 0.0f;

		TransitionNodeState TransitionState = TransitionNodeState::BeginTransition;

		Fracture::UUID CurrentState;
		Fracture::UUID TargetState;

		Fracture::UUID entryCondiitonNode;
		BoolValueNode* mEntryConditionNode;
	};





}

#endif
