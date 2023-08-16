#include "FracturePCH.h"
#include "AnimationGraphSerialiser.h"
#include "Animation/AnimationGraph.h"
#include "Animation/AnimationState.h"

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

void Fracture::AnimationGraphSerialiser::WriteConditions(AnimationGraph* graph)
{
	for (const auto& conditions: graph->Conditions)
	{
		for (const auto& condition : conditions.second)
		{
			BeginStruct("Condition");
			Property("ParameterID", conditions.first);			
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
	for (const auto& parameter : graph->Transitions)
	{
		BeginStruct("Transition");
		Property("ParameterID", parameter.first);
		Property("Name", parameter.second->Name);
		Property("From", parameter.second->FromState);
		Property("To", parameter.second->ToState);
		EndStruct();
	}
}

std::shared_ptr<Fracture::AnimationGraph> Fracture::AnimationGraphSerialiser::ReadGraph()
{
	auto graph = std::make_shared<AnimationGraph>();




	return graph;
}
