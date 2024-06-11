#pragma once
#ifndef ANIMATIONGRAPH_H
#define ANIMATIONGRAPH_H

#include "GraphNodes/IAnimationNode.h"

namespace Fracture
{
	struct AnimationState;
	struct StateTransition;
	struct AnimationParameter;
	struct AnimationCondition;
	struct IAnimationTask;
	class AnimationSystem; 
	struct AnimationPose;

	struct AnimationGraph
	{
		AnimationGraph(AnimationSystem* sys);
		std::string Name = "NewGraph";
		Fracture::UUID ID;
		Fracture::UUID Current_StateID = -1;
		bool IsDirty = true;
		
		std::vector<std::unique_ptr<IAnimationNode>> GraphNodes;
		std::vector<NodeLink> Links;
		std::map<Fracture::UUID, std::shared_ptr<AnimationParameter>> Parameters;

		

		AnimationState* GetCurrentState();

		void EvaluateStateTransitions();

		void SetCurrentState(Fracture::UUID state_id);
		
		void TopologicalSort();

		void Process(AnimationContext& context);

		void PushTask(std::shared_ptr<IAnimationTask> task);

		void ExecuteTaskList();

		void OnUpdate(float time, Fracture::UUID entityid);

		void CalculateFinalGlobalTransforms();

		std::vector<PoseSample>& GetPoseBufferAndSwap();

		std::vector<PoseSample>& GetGlobalPoseBuffer(int index);

		void SetPoseBuffer(std::vector<PoseSample> new_buffer);

		IAnimationNode* GetNode(UUID node_id);

		bool EntityandMeshSet = true;
		int GlobalIndex = 0;

		std::vector<glm::mat4> mLocalTansforms;
		std::vector<glm::mat4> mGlobalTansforms;

		inline AnimationContext* GetContext() { return mContext.get(); }

	private:
		std::unordered_map<UUID, std::vector<UUID>> adjList;
		std::vector<IAnimationNode*> m_ExecutionOrder;
		std::unique_ptr<AnimationContext> mContext;
		void DepthFirstSearch();

		std::vector<std::vector<PoseSample>> mPoseBuffer;

		std::queue<std::shared_ptr<IAnimationTask>> mAnimationTasks;


		bool mSwapBuffers = false;


		AnimationSystem* mSystem;

	};



}

#endif 