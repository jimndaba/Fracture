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
			WriteLinks(*link.get());
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
				Property("Value", parameter.second->Value.BOOLEAN);
				break;
			}
			case Fracture::AnimationParameter::ParameterValueType::Float:
			{
				Property("Value", parameter.second->Value.FLOAT);
				break;
			}
			case Fracture::AnimationParameter::ParameterValueType::Int:
			{
				Property("Value", parameter.second->Value.INTERGER);
				break;
			}
			case Fracture::AnimationParameter::ParameterValueType::Trigger:
			{
				Property("Value", parameter.second->Value.TRIGGER);
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
		WriteNodeIfType<AndNode>(node, "AndNode");
		WriteNodeIfType<OrNode>(node, "OrNode");
		WriteNodeIfType<NotNode>(node, "NotNode");
		WriteNodeIfType<FloatEqualsNode>(node, "FloatEqualsNode");
		WriteNodeIfType<FloatGreaterThanNode>(node, "FloatGreaterThanNode");
		WriteNodeIfType<FloatLessThanNode>(node, "FloatLessThanNode");
		WriteNodeIfType<FloatClampNode>(node, "FloatClampNode");
		WriteNodeIfType<FloatSelectorNode>(node, "FloatSelectorNode");
		WriteNodeIfType<AnimationPoseNode>(node, "AnimationPoseNode");
		WriteNodeIfType<ReferencePoseNode>(node, "ReferencePoseNode");
		WriteNodeIfType<PoseSelectorNode>(node, "PoseSelectorNode");
		WriteNodeIfType<Blend1DNode>(node, "Blend1DNode");
		WriteNodeIfType<FloatValueNode>(node, "FloatValueNode");
		WriteNodeIfType<IntValueNode>(node, "IntValueNode");
		WriteNodeIfType<BoolValueNode>(node, "BoolValueNode");
		WriteNodeIfType<AnimationClipNode>(node, "AnimationClipNode");
		WriteNodeIfType<AnimationStateMachineNode>(node, "AnimationStateMachineNode");
		WriteNodeIfType<AnimationStateNode>(node, "AnimationStateNode");
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
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::OrNode* node)
{	
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::NotNode* node)
{

}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::ReferencePoseNode* node)
{
	Property("CurrentAnimationID",node->Result.AnimationClip);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AnimationPoseNode* node)
{
	Property("CurrentAnimationID", node->Result.AnimationClip);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::PoseSelectorNode* node)
{
	Property("Value", node->Value);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::Blend1DNode* node)
{
	Property("BlendFactor", node->BlendFactor);
	Property("BlendFunction", (int)node->BlendFunction);
	Property("BlendSpace", (int)node->BlendSpace);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatEqualsNode* node)
{
	Property("RHfloat", node->RHfloat);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatGreaterThanNode* node)
{
	Property("RHfloat", node->RHfloat);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatLessThanNode* node)
{
	Property("RHfloat", node->RHfloat);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatValueNode* node)
{
	Property("Name", node->Name);
	Property("ParameterID", node->ParameterID);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatSelectorNode* node)
{
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::BoolValueNode* node)
{
	Property("ParameterID", node->ParameterID);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::IntValueNode* node)
{
	Property("ParameterID", node->ParameterID);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::FloatClampNode* node)
{
	Property("Min", node->Min);
	Property("Max", node->Max);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AnimationClipNode* node)
{
	Property("AnimationID", node->Result.AnimationClip);
	Property("AnimationSet", node->AnimationSet);
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AnimationStateMachineNode* state_machine)
{
	Property("States", state_machine->mStates);
	Property("Transitions", state_machine->mTransitions);
	Property("Default State", state_machine->Default_State);

	BeginCollection("Nodes");
	for (const auto& mNode : state_machine->mNodes)
	{
		const auto& node = mNode.get();
		WriteNodeIfType<AndNode>(node, "AndNode");
		WriteNodeIfType<OrNode>(node, "OrNode");
		WriteNodeIfType<NotNode>(node, "NotNode");
		WriteNodeIfType<FloatEqualsNode>(node, "FloatEqualsNode");
		WriteNodeIfType<FloatGreaterThanNode>(node, "FloatGreaterThanNode");
		WriteNodeIfType<FloatLessThanNode>(node, "FloatLessThanNode");
		WriteNodeIfType<FloatClampNode>(node, "FloatClampNode");
		WriteNodeIfType<FloatSelectorNode>(node, "FloatSelectorNode");
		WriteNodeIfType<AnimationPoseNode>(node, "AnimationPoseNode");
		WriteNodeIfType<ReferencePoseNode>(node, "ReferencePoseNode");
		WriteNodeIfType<PoseSelectorNode>(node, "PoseSelectorNode");
		WriteNodeIfType<Blend1DNode>(node, "Blend1DNode");
		WriteNodeIfType<FloatValueNode>(node, "FloatValueNode");
		WriteNodeIfType<IntValueNode>(node, "IntValueNode");
		WriteNodeIfType<BoolValueNode>(node, "BoolValueNode");
		WriteNodeIfType<AnimationClipNode>(node, "AnimationClipNode");
		WriteNodeIfType<AnimationStateMachineNode>(node, "AnimationStateMachineNode");
		WriteNodeIfType<AnimationStateNode>(node, "AnimationStateNode");
	}
	EndCollection();
	
	BeginCollection("Links");	
	for (const auto& link : state_machine->Links)
	{
		WriteLinks(link);
	}
	EndCollection();
}

void Fracture::AnimationGraphSerialiser::WriteGraphNode(Fracture::AnimationStateNode* node)
{
	Property("IsLooping", node->Looping);
	Property("AnimationID", node->Result.AnimationClip);
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

		ReadLinks(graph.get());

		ReadNodes(graph.get());
	
		EndStruct();

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

				switch (parameter->ValueType) {
					case AnimationParameter::ParameterValueType::Bool:
					{
						parameter->Value.BOOLEAN = BOOL("Value");
						break;
					}
					case AnimationParameter::ParameterValueType::Float:
					{
						parameter->Value.FLOAT = FLOAT("Value");
						break;
					}
					case AnimationParameter::ParameterValueType::Int:
					{
						parameter->Value.INTERGER = INT("Value");
						break;
					}
					case AnimationParameter::ParameterValueType::Trigger:
					{
						parameter->Value.TRIGGER = BOOL("Value");
						break;
					}

				}
				graph->Parameters[ID("ID")] = parameter;
				EndStruct();
			}
			NextInCollection();
		}
		EndCollection();
	}
}

void Fracture::AnimationGraphSerialiser::ReadLinks(AnimationGraph* graph)
{
	if (BeginCollection("Links"))
	{
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			if (BeginStruct("Link"))
			{
				auto link = std:: make_shared<NodeLink>();
				link->ID = ID("ID");
				link->FromPinID = ID("FromPin");
				link->ToPinID = ID("ToPin");
				link->NodeFrom = ID("FromNode");
				link->NodeTo = ID("ToNode");
				graph->Links.push_back(link);
				EndStruct();
			}
			NextInCollection();
		}
		EndCollection();
	}
}

void Fracture::AnimationGraphSerialiser::ReadNodes(AnimationGraph* graph)
{
	if (BeginCollection("Nodes"))
	{
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			ReadAndNodeIfExists(graph);
			ReadOrNodeIfExists(graph);
			ReadNotNodeIfExists(graph);
			ReadReferencePoseNodeIfExists(graph);
			ReadAnimationPoseNodeIfExists(graph);
			ReadPoseSelectorNodeIfExists(graph);
			ReadBlend1DNodeIfExists(graph);
			ReadFloatEqualsNodeIfExists(graph);
			ReadFloatGreaterNodeIfExists(graph);
			ReadFloatLessThanNodeIfExists(graph);
			ReadFloatValueNodeIfExists(graph);
			ReadBoolValueNodeIfExists(graph);
			ReadIntValueNodeIfExists(graph);
			ReadFloatClampNodeIfExists(graph);
			ReadAnimationClipNodeIfExists(graph);
			ReadAnimationStateMachineNodeIfExists(graph);
			ReadAnimationStateNodeIfExists(graph);
			NextInCollection();
		}
		EndCollection();
	}
}

void Fracture::AnimationGraphSerialiser::ReadNodePinIDs(IAnimationNode* node)
{
	node->NodeID = ID("NodeID");
	if (BeginCollection("InputPins"))
	{
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			if (BeginStruct("Pin"))
			{
				node->InputPins[CurrentCollectionIndex()].PinID = ID("PinID");
				node->InputPins[CurrentCollectionIndex()].NodeID = ID("NodeID");
				EndStruct();
			}
			NextInCollection();
		}
		EndCollection();
	}
	if (BeginCollection("OutputPins"))
	{
		while (CurrentCollectionIndex() < GetCollectionSize())
		{
			if (BeginStruct("Pin"))
			{
				node->OutPins[CurrentCollectionIndex()].PinID = ID("PinID");
				node->OutPins[CurrentCollectionIndex()].NodeID = ID("NodeID");
				EndStruct();
			}
			NextInCollection();
		}
		EndCollection();
	}
}

void Fracture::AnimationGraphSerialiser::ReadAndNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("AndNode"))
	{
		auto node = std::make_unique<AndNode>();	
		ReadNodePinIDs(node.get());
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadOrNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("OrNode"))
	{
		auto node = std::make_unique<OrNode>();		
		ReadNodePinIDs(node.get());
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadNotNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("NotNode"))
	{
		auto node = std::make_unique<NotNode>();
		node->NodeID = ID("ID");

		ReadNodePinIDs(node.get());
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadReferencePoseNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("ReferencePoseNode"))
	{
		auto node = std::make_unique<ReferencePoseNode>();	
		ReadNodePinIDs(node.get());
		node->Result.AnimationClip = ID("CurrentAnimationID");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadAnimationPoseNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("AnimationPoseNode"))
	{
		auto node = std::make_unique<AnimationPoseNode>();		
		ReadNodePinIDs(node.get());
		node->Result.AnimationClip = ID("CurrentAnimationID");
		AssetManager::Instance()->AsyncLoadAnimationByID(node->Result.AnimationClip);
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadPoseSelectorNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("PoseSelectorNode"))
	{
		auto node = std::make_unique<PoseSelectorNode>();		
		node->Value = BOOL("Value");
		ReadNodePinIDs(node.get());
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadBlend1DNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("Blend1DNode"))
	{
		auto node = std::make_unique<Blend1DNode>();		
		ReadNodePinIDs(node.get());
		node->BlendFactor = FLOAT("BlendFactor");
		node->BlendFunction = (BlendFunctionType)INT("BlendFunction");
		node->BlendSpace = (BlendSpaceType) INT("BlendSpace");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadFloatEqualsNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("FloatEqualsNode"))
	{
		auto node = std::make_unique<FloatEqualsNode>();
		ReadNodePinIDs(node.get());
		node->RHfloat = FLOAT("RHfloat");		
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadFloatGreaterNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("FloatGreaterThanNode"))
	{
		auto node = std::make_unique<FloatGreaterThanNode>();		
		ReadNodePinIDs(node.get());
		node->RHfloat = FLOAT("RHfloat");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadFloatLessThanNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("FloatLessThanNode"))
	{
		auto node = std::make_unique<FloatLessThanNode>();		
		ReadNodePinIDs(node.get());
		node->RHfloat = FLOAT("RHfloat");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadFloatValueNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("FloatValueNode"))
	{
		auto node = std::make_unique<FloatValueNode>();		
		ReadNodePinIDs(node.get());
		node->Name = STRING("Name");
		node->ParameterID = ID("ParameterID");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadBoolValueNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("BoolValueNode"))
	{
		auto node = std::make_unique<BoolValueNode>();		
		ReadNodePinIDs(node.get());
		node->ParameterID = ID("ParameterID");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadIntValueNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("IntValueNode"))
	{
		auto node = std::make_unique<IntValueNode>();	
		ReadNodePinIDs(node.get());
		node->ParameterID = ID("ParameterID");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadFloatClampNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("FloatClampNode"))
	{
		auto node = std::make_unique<FloatClampNode>();		
		ReadNodePinIDs(node.get());
		node->Min = ID("Min");
		node->Max = ID("Max");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadAnimationClipNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("AnimationClipNode"))
	{
		auto node = std::make_unique<AnimationClipNode>();		
		ReadNodePinIDs(node.get());
		node->Result.AnimationClip = ID("AnimationID");
		node->AnimationSet = BOOL("AnimationSet");
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadAnimationStateMachineNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("AnimationStateMachineNode"))
	{
		auto node = std::make_unique<AnimationStateMachineNode>();
		ReadNodePinIDs(node.get());
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

void Fracture::AnimationGraphSerialiser::ReadAnimationStateNodeIfExists(AnimationGraph* graph)
{
	if (BeginStruct("AnimationStateNode"))
	{
		auto node = std::make_unique<AnimationStateNode>();		
		ReadNodePinIDs(node.get());
		graph->GraphNodes.push_back(std::move(node));
		EndStruct();
	}
}

