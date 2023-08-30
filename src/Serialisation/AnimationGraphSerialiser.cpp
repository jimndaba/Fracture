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

		BeginCollection("Parameters");
		WriteParameters(graph);
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

std::shared_ptr<Fracture::AnimationGraph> Fracture::AnimationGraphSerialiser::ReadGraph()
{
	if (BeginStruct("AnimationGraph"))
	{
		auto graph = std::make_shared<AnimationGraph>(AnimationSystem::Instance());
		graph->ID = ID("ID");
		graph->Name = STRING("Name");
		graph->Current_StateID = ID("CurrentState");
		
		ReadParameters(graph.get());
	
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

