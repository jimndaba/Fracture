#pragma once
#ifndef ANIMATIONGRAPHSERIALISER_H
#define ANIMATIONGRAPHSERIALISER_H


#include "Serialisation/Serialiser.h"

#include "Animation/GraphNodes/IAnimationNode.h"
#include "Animation/GraphNodes/ValueNodes.h"
#include "Animation/GraphNodes/SelectorNodes.h"
#include "Animation/GraphNodes/PoseNodes.h"
#include "Animation/GraphNodes/FloatNodes.h"
#include "Animation/GraphNodes/BooleanNodes.h"
#include "Animation/GraphNodes/BlendNodes.h"
#include "Animation/GraphNodes/AnimationTransitionNode.h"
#include "Animation/GraphNodes/AnimationStateNode.h"
#include "Animation/GraphNodes/AnimationStateMachineNode.h"
#include "Animation/GraphNodes/AnimationClipNode.h"

namespace Fracture
{
	struct AnimationGraph;

	struct AnimationGraphSerialiser : public ISerialiser
	{
		AnimationGraphSerialiser(ISerialiser::IOMode mode, ISerialiser::SerialiseFormat format);
		void WriteGraph(AnimationGraph* clip);

		void WriteParameters(AnimationGraph* graph);

		void WriteNodes(AnimationGraph* graph);

		void WriteLinks(const NodeLink& link);

		template <class T>
		void WriteNodeIfType(Fracture::IAnimationNode* node, const std::string& name);
			 
		void WriteGraphNode(Fracture::AndNode* node);
		void WriteGraphNode(Fracture::OrNode* node);
		void WriteGraphNode(Fracture::NotNode* node);
		void WriteGraphNode(Fracture::ReferencePoseNode* node);
		void WriteGraphNode(Fracture::AnimationPoseNode* node);
		void WriteGraphNode(Fracture::PoseSelectorNode* node);
		void WriteGraphNode(Fracture::Blend1DNode* node);
		void WriteGraphNode(Fracture::FloatEqualsNode* node);
		void WriteGraphNode(Fracture::FloatGreaterThanNode* node);
		void WriteGraphNode(Fracture::FloatLessThanNode* node);
		void WriteGraphNode(Fracture::FloatValueNode* node);
		void WriteGraphNode(Fracture::FloatSelectorNode* node);
		void WriteGraphNode(Fracture::BoolValueNode* node);
		void WriteGraphNode(Fracture::IntValueNode* node);
		void WriteGraphNode(Fracture::FloatClampNode* node);
		void WriteGraphNode(Fracture::AnimationClipNode* node);
		void WriteGraphNode(Fracture::AnimationStateMachineNode* node);
		void WriteGraphNode(Fracture::AnimationStateNode* node);
	

		std::shared_ptr<AnimationGraph> ReadGraph();
		void ReadParameters(AnimationGraph* graph);	
		void ReadLinks(AnimationGraph* graph);
		void ReadNodes(AnimationGraph* graph);

		void ReadNodePinIDs(IAnimationNode* node);
		void ReadAndNodeIfExists(AnimationGraph* graph);
		void ReadOrNodeIfExists(AnimationGraph* graph);
		void ReadNotNodeIfExists(AnimationGraph* graph);
		void ReadReferencePoseNodeIfExists(AnimationGraph* graph);
		void ReadAnimationPoseNodeIfExists(AnimationGraph* graph);
		void ReadPoseSelectorNodeIfExists(AnimationGraph* graph);
		void ReadBlend1DNodeIfExists(AnimationGraph* graph);
		void ReadFloatEqualsNodeIfExists(AnimationGraph* graph);
		void ReadFloatGreaterNodeIfExists(AnimationGraph* graph);
		void ReadFloatLessThanNodeIfExists(AnimationGraph* graph);
		void ReadFloatValueNodeIfExists(AnimationGraph* graph);
		void ReadBoolValueNodeIfExists(AnimationGraph* graph);
		void ReadIntValueNodeIfExists(AnimationGraph* graph);
		void ReadFloatClampNodeIfExists(AnimationGraph* graph);
		void ReadAnimationClipNodeIfExists(AnimationGraph* graph);
		void ReadAnimationStateMachineNodeIfExists(AnimationGraph* graph);
		void ReadAnimationStateNodeIfExists(AnimationGraph* graph);


	};

	template<class T>
	inline void AnimationGraphSerialiser::WriteNodeIfType(Fracture::IAnimationNode* node , const std::string& name)
	{
		if (dynamic_cast<T*>(node))
		{
			BeginStruct(name);
			Property("NodeID", node->NodeID);

			BeginCollection("InputPins");
			for (const auto& pin : node->InputPins)
			{

				BeginStruct("Pin");
				Property("PinID", pin.PinID);
				Property("NodeID", pin.NodeID);
				EndStruct();
			}
			EndCollection();

			BeginCollection("OutputPins");
			for (const auto& pin : node->OutPins)
			{

				BeginStruct("Pin");
				Property("PinID", pin.PinID);
				Property("NodeID", pin.NodeID);
				EndStruct();
			}
			EndCollection();

			WriteGraphNode(dynamic_cast<T*>(node));
			EndStruct();
		}
	}
}

#endif