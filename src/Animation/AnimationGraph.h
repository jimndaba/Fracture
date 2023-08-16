#pragma once
#ifndef ANIMATIONGRAPH_H
#define ANIMATIONGRAPH_H


namespace Fracture
{
	struct AnimationState;
	struct StateTransition;
	struct AnimationParameter;
	struct AnimationCondition;

	struct AnimationGraph
	{
		AnimationGraph();
		std::string Name;
		Fracture::UUID ID;
		Fracture::UUID Current_StateID = -1;

		std::map<Fracture::UUID, std::shared_ptr<AnimationState>> States;
		std::map<Fracture::UUID,std::shared_ptr<StateTransition>> Transitions;
		std::map<Fracture::UUID,std::shared_ptr<AnimationParameter>> Parameters;
		std::map<Fracture::UUID,std::vector<std::shared_ptr<AnimationCondition>>> Conditions;

		void EvaluateStateTransitions();

		void SetCurrentState(Fracture::UUID state_id);

		AnimationState* GetCurrentState();
		bool IsDirty = false;
	};



}

#endif 