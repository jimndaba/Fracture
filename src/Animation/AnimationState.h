#pragma once
#ifndef ANIMATIONSTATE_H
#define ANIMATIONSTATE_H


namespace Fracture
{
	struct AnimationState
	{
		Fracture::UUID ID;
		Fracture::UUID ParentID;
		Fracture::UUID ClipID;
		
		std::string Name;
		std::vector<Fracture::UUID> SubStates;
		float mTime = 0.0f;
		float mWeight = 1.0f;
		bool Looping;
		bool Enabled;
		bool HasClip;
	};

	struct StateTransition
	{
		Fracture::UUID ID;
		std::string Name;
		Fracture::UUID FromState;
		Fracture::UUID ToState;
	};

	struct AnimationParameter
	{
		enum class ParameterValueType
		{
			Float,
			Int,
			Bool,
			Trigger,
			Count
		};

		union ParameterValue
		{
			float FLOAT;
			bool BOOLEAN;
			int INTERGER;
			bool TRIGGER;
		};

		std::string Name;
		ParameterValueType ValueType;
		ParameterValue Value;
	};

	struct AnimationCondition
	{
		enum class ConditionOperation
		{
			Value,
			Equals,
			NotEquals,
			GreaterThan,
			LessThan,
			GreaterOrEqual,
			LessOrEqual,
			count
		};

		enum class ConditionValueType
		{
			Float,
			Int,
			Bool,
			Trigger,
			count
		};

		union ConditionValue
		{
			float FLOAT;
			bool BOOLEAN;
			int INTERGER;
			bool TRIGGER;
		};

		Fracture::UUID ParameterID;
		ConditionValueType ValueType;
		ConditionOperation OperationType;
		ConditionValue Comparison_Value;
		bool Result;
	};
}

#endif