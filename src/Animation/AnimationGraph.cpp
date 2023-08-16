#include "FracturePCH.h"
#include "AnimationGraph.h"
#include "AnimationState.h"

Fracture::AnimationGraph::AnimationGraph()
{

	auto mEntry_State = std::make_shared<AnimationState>();
	mEntry_State->Name = "Entry";
	mEntry_State->ID = UUID();
	States[mEntry_State->ID] = mEntry_State;
	SetCurrentState(mEntry_State->ID);
}

void Fracture::AnimationGraph::EvaluateStateTransitions()
{	
	if (Current_StateID < 0)
		return;

	auto current_state = States[Current_StateID];

	if (Conditions.find(Current_StateID) == Conditions.end())
	{
		for (const auto& transition : Transitions)
		{
			if (transition.second->FromState == Current_StateID)
			{	
				if(!States[Current_StateID]->Enabled)
					Current_StateID = transition.second->ToState;
				return;
			}
		}
	}
	else
	{

	}
	

}

void Fracture::AnimationGraph::SetCurrentState(Fracture::UUID state_id)
{
	Current_StateID = state_id;
	States[Current_StateID]->Enabled = true;
}

Fracture::AnimationState* Fracture::AnimationGraph::GetCurrentState()
{
	if (Current_StateID < 0)
		return nullptr;

	return States[Current_StateID].get();
}
