#include "FracturePCH.h"
#include "AnimationTransitionNode.h"
#include "Animation/AnimationGraph.h"
#include "Animation/AnimationTasks.h"

Fracture::AnimationTransitionNode::AnimationTransitionNode()
{
	

}

void Fracture::AnimationTransitionNode::Process(AnimationContext& context)
{
	const auto& current_state = (IPoseNode*)context._graph->GetNode(CurrentState);
	const auto& target_state = (IPoseNode*)context._graph->GetNode(TargetState);
	
	TotalTransitionTime = current_state->Duration + target_state->Duration - TransitionOffset;

	if (TransitionState == TransitionNodeState::BeginTransition)
	{
		Result.PoseNodeID = NodeID;
		start_transition = current_state->Duration - TransitionOffset;
		end_transition = current_state->Duration;	
		TransitionState = TransitionNodeState::Transitioning;
	}

	if (TransitionState == TransitionNodeState::Transitioning)
	{		
		if (Time <= start_transition)
		{	
			std::shared_ptr<SampleTask> task = std::make_shared<SampleTask>();
			task->Time = Time;
			task->ClipID = current_state->Result.AnimationClip;
			task->NodeID = CurrentState;
			context._graph->PushTask(task);
		}

		if (Time > start_transition && Time < end_transition)
		{
			float factor = Time / TransitionOffset;
			{
				std::shared_ptr<SampleTask> task_pose1 = std::make_shared<SampleTask>();
				task_pose1->Time = Time;
				task_pose1->ClipID = current_state->Result.AnimationClip;
				task_pose1->NodeID = CurrentState;
				context._graph->PushTask(task_pose1);
			}
			{
				std::shared_ptr<SampleTask> task_pose2 = std::make_shared<SampleTask>();
				task_pose2->Time = Time;
				task_pose2->ClipID = current_state->Result.AnimationClip;
				task_pose2->NodeID = TargetState;
				context._graph->PushTask(task_pose2);
			}
			{
				std::shared_ptr<BlendTask> blend = std::make_shared<BlendTask>();
				blend->Pose1 = CurrentState;
				blend->Pose2 = TargetState;
				blend->factor = factor;
				context._graph->PushTask(blend);
			}
		}

		if (Time > end_transition && Time < TotalTransitionTime)
		{
			std::shared_ptr<SampleTask> task = std::make_shared<SampleTask>();
			task->Time = Time;
			task->ClipID = target_state->Result.AnimationClip;
			task->NodeID = TargetState;
			context._graph->PushTask(task);
		}

		if (Time > TotalTransitionTime)
		{
			TransitionState = TransitionNodeState::EndTransition;
		}
	}

	if (TransitionState == TransitionNodeState::EndTransition)
	{
		Result.PoseNodeID = TargetState;
	}

}
