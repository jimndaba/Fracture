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

		Fracture::UUID NodeID;
		Fracture::UUID Resource;
	};

	struct NodeLink
	{
		Fracture::UUID ID;
		Fracture::UUID FromID;
		Fracture::UUID ToID;

		Fracture::UUID NodeFrom;
		Fracture::UUID NodeTo;
		Fracture::UUID Resource;
		inline NodeLink operator = (const int& other) { ID = other; return *this; }
	};

	struct PoseSample
	{
		glm::vec3 Position;
		glm::vec3 Scale;
		glm::quat Rotation;
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
		union NodeResult
		{
			bool BOOL;
			int INT;
			float FLOAT;	
		};

		IAnimationNode();

		std::string Name;
		Fracture::UUID NodeID;
		NodeValueType ValueType;
		NodeResult Result;

		Fracture::UUID PoseNodeID;

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

		Fracture::UUID AnimationClipID;
		bool AnimationSet = false;
		bool Looping = false;
		bool Enabled = false;
		float Time;
		float Duration;

	};
}


#endif
