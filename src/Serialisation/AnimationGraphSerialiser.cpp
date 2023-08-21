#include "FracturePCH.h"
#include "AnimationGraphSerialiser.h"
#include "Animation/AnimationGraph.h"
#include "Animation/AnimationState.h"
#include "Animation/AnimationSystem.h"

Fracture::AnimationGraphSerialiser::AnimationGraphSerialiser(ISerialiser::IOMode mode, ISerialiser::SerialiseFormat format):
	ISerialiser(mode,format)
{
}

void Fracture::AnimationGraphSerialiser::WriteGraph(AnimationGraph* graph)
{
	BeginStruct("AnimationGraph");
	{
		Property("ID", graph->ID);
		Property("Name", graph->Name);
		Property("CurrentState", graph->Current_StateID);

		BeginCollection("States");
		WriteStates(graph);
		EndCollection();

		BeginCollection("Parameters");
		WriteParameters(graph);
		EndCollection();

		BeginCollection("Conditions");
		WriteConditions(graph);
		EndCollection();

		BeginCollection("Transitions");
		WriteTransitions(graph);
		EndCollection();
	}
	EndStruct();

}

void Fracture::AnimationGraphSerialiser::WriteParameters(AnimationGraph* graph)
{
	for (const auto& parameter : graph->Parameters)
	{
		BeginStruct("Parameter");
		Property("Name", parameter.second->Name);
		Property("Type", (int)parameter.second->ValueType);
		Property("ID", parameter.first);
		switch (parameter.second->ValueType)
		{
			case Fracture::AnimationParameter::ParameterValueType::Bool:
			{
				Property("Value", (int)parameter.second->Value.BOOLEAN);
				break;
			}
			case Fracture::AnimationParameter::ParameterValueType::Float:
			{
				Property("Value", (int)parameter.second->Value.FLOAT);
				break;
			}
			case Fracture::AnimationParameter::ParameterValueType::Int:
			{
				Property("Value", (int)parameter.second->Value.INTERGER);
				break;
			}
			case Fracture::AnimationParameter::ParameterValueType::Trigger:
			{
				Property("Value", (int)parameter.second->Value.INTERGER);
				break;
			}
		}
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::WriteStates(AnimationGraph* graph)
{
	for (const auto& state : graph->States)
	{
		BeginStruct("State");
		Property("Name", state.second->Name);
		Property("IsLooping", state.second->Looping);
		Property("Weight", state.second->mWeight);
		Property("Parent", state.second->ParentID);
		Property("Animation", state.second->ClipID);
		Property("HasAnimation", state.second->HasClip);
		Property("ID", state.first);
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::WriteConditions(AnimationGraph* graph)
{
	for (const auto& conditions: graph->Conditions)
	{
		for (const auto& condition : conditions.second)
		{
			BeginStruct("Condition");
			Property("ParameterID", condition->ParameterID);			
			Property("TransitionID", conditions.first);
			Property("Operation",(int)condition->OperationType);
			Property("ValueType", (int)condition->ValueType);
			switch (condition->ValueType)
			{
				case Fracture::AnimationCondition::ConditionValueType::Bool:
				{
					Property("ComparisonValue", condition->Comparison_Value.BOOLEAN);
					break;
				}
				case Fracture::AnimationCondition::ConditionValueType::Int:
				{
					Property("ComparisonValue", condition->Comparison_Value.INTERGER);
					break;
				}
				case Fracture::AnimationCondition::ConditionValueType::Float:
				{
					Property("ComparisonValue", condition->Comparison_Value.FLOAT);
					break;
				}
				case Fracture::AnimationCondition::ConditionValueType::Trigger:
				{
					Property("ComparisonValue", condition->Comparison_Value.TRIGGER);
					break;
				}
			}			
			EndStruct();
		}
	}
}

void Fracture::AnimationGraphSerialiser::WriteTransitions(AnimationGraph* graph)
{
	for (const auto& transition : graph->Transitions)
	{
		BeginStruct("Transition");
		Property("TransitionID", transition.first);
		Property("Name", transition.second->Name);
		Property("From", transition.second->FromState);
		Property("To", transition.second->ToState);
		EndStruct();
	}
}

std::shared_ptr<Fracture::AnimationGraph> Fracture::AnimationGraphSerialiser::ReadGraph()
{
	if (BeginStruct("AnimationGraph"))
	{
		auto graph = std::make_shared<AnimationGraph>(AnimationSystem::Instance());
		graph->ID = ID("ID");
		graph->Name = STRING("Name");
		graph->Current_StateID = ID("CurrentState");
		ReadStates(graph.get());
		ReadParameters(graph.get());
		ReadConditions(graph.get());
		ReadTransitions(graph.get());
		return graph;
	}
	return nullptr;
}

void Fracture::AnimationGraphSerialiser::ReadParameters(AnimationGraph* graph)
{
	if (BeginCollection("Parameters"))
	{
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			if (BeginStruct("Parameter"))
			{
				auto parameter = std::make_shared<AnimationParameter>();

				parameter->Name = STRING("Name");
				parameter->ValueType = (AnimationParameter::ParameterValueType)INT("Type");			
				graph->Parameters[ID("ID")] = parameter;
				EndStruct();
			}
			NextInCollection();
		}
		EndCollection();
	}
}

void Fracture::AnimationGraphSerialiser::ReadStates(AnimationGraph* graph)
{
	if (BeginCollection("States"))
	{
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			if (BeginStruct("State"))
			{
				auto state = std::make_shared<AnimationState>();
				state->Name = STRING("Name");
				state->ID = ID("ID");
				state->ParentID = ID("Parent");			
				state->HasClip = BOOL("HasAnimation");
				state->ClipID = ID("Animation");
				state->Looping = BOOL("IsLooping");
				state->mWeight = FLOAT("Weight");
				graph->States[state->ID] = state;
				EndStruct();
			}
			NextInCollection();
		}
		EndCollection();
	}

}

void Fracture::AnimationGraphSerialiser::ReadConditions(AnimationGraph* graph)
{
	if (BeginCollection("Conditions"))
	{
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			if (BeginStruct("Condition"))
			{
				auto condition = std::make_shared<AnimationCondition>();

				condition->ParameterID = ID("ParameterID");
				condition->OperationType = (AnimationCondition::ConditionOperation)INT("Operation");
				condition->ValueType = (AnimationCondition::ConditionValueType)INT("ValueType");

				switch (condition->ValueType)
				{
					case AnimationCondition::ConditionValueType::Bool:
					{
						condition->Comparison_Value.BOOLEAN = BOOL("ComparisonValue");
						break;
					}
					case AnimationCondition::ConditionValueType::Float:
					{
						condition->Comparison_Value.FLOAT = FLOAT("ComparisonValue");
						break;
					}
					case AnimationCondition::ConditionValueType::Int:
					{
						condition->Comparison_Value.INTERGER = INT("ComparisonValue");
						break;
					}
					case AnimationCondition::ConditionValueType::Trigger:
					{
						condition->Comparison_Value.TRIGGER = BOOL("ComparisonValue");
						break;
					}
				}

				graph->Conditions[ID("TransitionID")].push_back(condition);
				EndStruct();
			}
			NextInCollection();
		}
		EndCollection();
	}
}

void Fracture::AnimationGraphSerialiser::ReadTransitions(AnimationGraph* graph)
{
	if (BeginCollection("Transitions"))
	{
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			if (BeginStruct("Transition"))
			{
				auto transition = std::make_shared<StateTransition>();
				transition->ID = ID("TransitionID");
				transition->Name = STRING("Name");
				transition->FromState = ID("From");
				transition->ToState = ID("To");
				graph->Transitions[transition->ID] = transition;
				EndStruct();
			}
			NextInCollection();
		}
		EndCollection();
	}
}
