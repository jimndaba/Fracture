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


		BeginCollection("Nodes");
		WriteNodes(graph);
		EndCollection();

		BeginCollection("Links");
		for(const auto& link : graph->Links)
		{
			WriteLinks(link);
		}
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

void Fracture::AnimationGraphSerialiser::WriteNodes(AnimationGraph* graph)
{
	for (const auto& mNode : graph->GraphNodes)
	{
		const auto& node = mNode.get();
		WriteNodeIfType<AndNode>(node);
		WriteNodeIfType<OrNode>(node);
		WriteNodeIfType<NotNode>(node);
		WriteNodeIfType<FloatEqualsNode>(node);
		WriteNodeIfType<FloatGreaterThanNode>(node);
		WriteNodeIfType<FloatLessThanNode>(node);
		WriteNodeIfType<FloatClampNode>(node);
		WriteNodeIfType<FloatSelectorNode>(node);
		WriteNodeIfType<AnimationPoseNode>(node);
		WriteNodeIfType<ReferencePoseNode>(node);
		WriteNodeIfType<PoseSelectorNode>(node);
		WriteNodeIfType<Blend1DNode>(node);
		WriteNodeIfType<FloatValueNode>(node);
		WriteNodeIfType<IntValueNode>(node);
		WriteNodeIfType<BoolValueNode>(node);
		WriteNodeIfType<AnimationClipNode>(node);
		WriteNodeIfType<AnimationStateMachineNode>(node);
		WriteNodeIfType<AnimationStateNode>(node);
	}
}

void Fracture::AnimationGraphSerialiser::WriteLinks(const NodeLink& link)
{
	BeginStruct("Link");
	Property("ID", link.ID);
	Property("FromPin", link.FromPinID);
	Property("ToPin", link.ToPinID);
	Property("FromNode", link.NodeFrom);
	Property("ToNode", link.NodeTo);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AndNode* node)
{
	BeginStruct("AndNode");
	Property("NodeID", node->NodeID);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::OrNode* node)
{
	BeginStruct("OrNode");
	Property("NodeID", node->NodeID);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::NotNode* node)
{
	BeginStruct("NotNode");
	Property("NodeID", node->NodeID);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::ReferencePoseNode* node)
{
	BeginStruct("ReferencePoseNode");
	Property("NodeID", node->NodeID);
	Property("CurrentAnimationID",node->CurrentAnimation);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AnimationPoseNode* node)
{
	BeginStruct("AnimationPoseNode");
	Property("NodeID", node->NodeID);
	Property("CurrentAnimationID", node->CurrentAnimation);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::PoseSelectorNode* node)
{
	BeginStruct("PoseSelectorNode");
	Property("NodeID", node->NodeID);
	Property("Value", node->Value);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::Blend1DNode* node)
{
	BeginStruct("Blend1DNode");
	Property("NodeID", node->NodeID);
	Property("BlendFactor", node->BlendFactor);
	Property("BlendFunction", (int)node->BlendFunction);
	Property("BlendSpace", (int)node->BlendSpace);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatEqualsNode* node)
{
	BeginStruct("FloatEqualsNode");
	Property("NodeID", node->NodeID);
	Property("RHfloat", node->RHfloat);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatGreaterThanNode* node)
{
	BeginStruct("FloatGreaterThanNode");
	Property("RHfloat", node->RHfloat);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatLessThanNode* node)
{
	BeginStruct("FloatLessThanNode");
	Property("RHfloat", node->RHfloat);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatValueNode* node)
{
	BeginStruct("FloatValueNode");
	Property("NodeID", node->NodeID);
	Property("ParameterID", node->ParameterID);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatSelectorNode* node)
{
	BeginStruct("FloatSelectorNode");
	Property("NodeID", node->NodeID);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::BoolValueNode* node)
{
	BeginStruct("BoolValueNode");
	Property("NodeID", node->NodeID);
	Property("ParameterID", node->ParameterID);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::IntValueNode* node)
{
	BeginStruct("IntValueNode");
	Property("NodeID", node->NodeID);
	Property("ParameterID", node->ParameterID);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatClampNode* node)
{
	BeginStruct("FloatClampNode");
	Property("NodeID", node->NodeID);
	Property("Min", node->Min);
	Property("Max", node->Max);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AnimationClipNode* node)
{
	BeginStruct("AnimationClipNode");
	Property("NodeID", node->NodeID);
	Property("AnimationID", node->CurrentAnimation);
	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AnimationStateMachineNode* state_machine)
{
	BeginStruct("AnimationStateMachineNode");
	
	Property("NodeID", state_machine->NodeID);
	Property("States", state_machine->mStates);
	Property("Transitions", state_machine->mTransitions);
	Property("Default State", state_machine->Default_State);

	BeginCollection("Nodes");
	for (const auto& mNode : state_machine->mNodes)
	{
		const auto& node = mNode.get();
		WriteNodeIfType<AndNode>(node);
		WriteNodeIfType<OrNode>(node);
		WriteNodeIfType<NotNode>(node);
		WriteNodeIfType<FloatEqualsNode>(node);
		WriteNodeIfType<FloatGreaterThanNode>(node);
		WriteNodeIfType<FloatLessThanNode>(node);
		WriteNodeIfType<FloatClampNode>(node);
		WriteNodeIfType<FloatSelectorNode>(node);
		WriteNodeIfType<AnimationPoseNode>(node);
		WriteNodeIfType<ReferencePoseNode>(node);
		WriteNodeIfType<PoseSelectorNode>(node);
		WriteNodeIfType<Blend1DNode>(node);
		WriteNodeIfType<FloatValueNode>(node);
		WriteNodeIfType<IntValueNode>(node);
		WriteNodeIfType<BoolValueNode>(node);
		WriteNodeIfType<AnimationClipNode>(node);
		WriteNodeIfType<AnimationStateMachineNode>(node);
		WriteNodeIfType<AnimationStateNode>(node);
	}
	EndCollection();
	
	BeginCollection("Links");	
	for (const auto& link : state_machine->Links)
	{
		WriteLinks(link);
	}
	EndCollection();


	EndStruct();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AnimationStateNode* node)
{
	BeginStruct("AnimationStateNode");
	Property("NodeID", node->NodeID);
	Property("IsLooping", node->Looping);
	Property("AnimationID", node->CurrentAnimation);
	EndStruct();
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

