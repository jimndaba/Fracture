#pragma once
#ifndef IANIMNODE_H
#define IANIMNODE_H

#include "Animation/AnimationPose.h"

namespace Fracture
{
	struct AnimationGraph;
	class AnimationSystem;

	enum class PinValueType
	{
		Float,
		Int,
		Bool,
		Pose,
		Bone
	};

	enum class BlendFunctionType
	{
		Additive,
		Multiply,
		Subtract
	};

	enum class BlendSpaceType
	{
		Blend1D,
		Blend2D
	};

	struct Pin
	{
		Fracture::UUID PinID;
		std::string Name;
		PinValueType PinType;
		bool IsValueSet = false;
		void* Value;

		Fracture::UUID NodeID;
		Fracture::UUID Resource;
	};

	struct NodeLink
	{
		enum class LinkConnectionType
		{
			NodeLink,
			TransitionLink
		};
		Fracture::UUID ID;

		Fracture::UUID FromPinID;
		Fracture::UUID ToPinID;

		Fracture::UUID NodeFrom;
		Fracture::UUID NodeTo;

		LinkConnectionType LinkType = LinkConnectionType::NodeLink;

		inline NodeLink operator = (const int& other) { ID = other; return *this; }
	};

	struct PoseSample
	{
		glm::vec3 Position = glm::vec3(0);
		glm::vec3 Scale = glm::vec3(0);
		glm::quat Rotation = glm::quat();
	};

	struct AnimationContext
	{
		Fracture::UUID EntityID;
		Fracture::UUID GraphID;
		Fracture::UUID MeshID; //Currenty Bones are kept in mesh, should extract on load and create skeleton from it?
		float Time;
		AnimationGraph* _graph;
		AnimationSystem* _system;
		//Pose??
	};

	enum class NodeValueType
	{
		BOOL,
		INTERGER,
		FLOAT,
		POSE
	};

	struct IAnimationNode
	{
		struct NodeResult
		{
			bool BOOL;
			int INT;
			float FLOAT;	
			Fracture::UUID AnimationClip;
			Fracture::UUID PoseNodeID;
		};

		IAnimationNode();

		std::string Name;
		Fracture::UUID NodeID;
		NodeValueType ValueType;
		NodeResult Result;

		std::vector<Pin> InputPins;
		std::vector<Pin> OutPins;
	

		//Sorting 
		bool isVisited = false;
		int LinkCount = 0;

		inline NodeValueType GetValueType() {return ValueType;}
		inline void SetValueType(NodeValueType mType) { ValueType = mType; };

		virtual void Process(AnimationContext& context) = 0;
		virtual void LinkResources() {};
	};

	struct IOperationNode : IAnimationNode
	{
		IOperationNode();
		void Process(AnimationContext& context) {};
	};

	struct IPoseNode : IAnimationNode
	{
		IPoseNode();
		void Process(AnimationContext& context) {};	
		Fracture::UUID CurrentAnimation;
		bool HasAnimation = false;
		bool Looping = false;
		bool Enabled = false;
		float Time = 0.0f;
		float Duration = 0.0f;
	};
}


#endif
