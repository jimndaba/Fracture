#include "FracturePCH.h"
#include "AnimationGraph.h"
#include "AnimationState.h"
#include "AnimationPose.h"
#include "AnimationTasks.h"
#include "Assets/AssetManager.h"
#include "AnimationSystem.h"

Fracture::AnimationGraph::AnimationGraph(AnimationSystem* sys):
	mSystem(sys)
{
	mContext = std::make_unique<AnimationContext>();
	mContext->_graph = this;
	mContext->_system = sys;
	mContext->GraphID = ID;
	mPoseBuffer.resize(2);
}

void Fracture::AnimationGraph::EvaluateStateTransitions()
{	
	if (Current_StateID < 0)
		return;

	auto current_state = States[Current_StateID];

	for (const auto& transition : Transitions)
	{
		//BeginTransition();
		if (Conditions.find(transition.first) == Conditions.end())
		{
			if (!current_state->HasClip || !current_state->Enabled)
			{
				if (transition.second->FromState == current_state->ID)
				{
					Current_StateID = transition.second->ToState;
					States[Current_StateID]->Enabled = true;
					States[Current_StateID]->mTime = 0;

				}			
			}
			else
			{
				if (!current_state->Enabled)
				{
					Current_StateID = transition.second->ToState;
					States[Current_StateID]->Enabled = true;
					States[Current_StateID]->mTime = 0;
				}
			}
		}
		else
		{
			if (transition.second->FromState == current_state->ID)
			{
				bool should_transition = true;
				for (auto condition : Conditions[transition.first])
				{		
					switch (condition->OperationType)
					{
						case AnimationCondition::ConditionOperation::Equals:
						{
							switch (condition->ValueType)
							{
								case AnimationCondition::ConditionValueType::Bool:
								{
									should_transition = should_transition && condition->Comparison_Value.BOOLEAN == Parameters[condition->ParameterID]->Value.BOOLEAN;
									break;
								}
								case AnimationCondition::ConditionValueType::Float:
								{
									should_transition = should_transition && condition->Comparison_Value.FLOAT == Parameters[condition->ParameterID]->Value.FLOAT;
									break;
								}
								case AnimationCondition::ConditionValueType::Int:
								{
									should_transition = should_transition && condition->Comparison_Value.INTERGER == Parameters[condition->ParameterID]->Value.INTERGER;
									break;
								}
								case AnimationCondition::ConditionValueType::Trigger:
								{
									should_transition = should_transition && condition->Comparison_Value.TRIGGER == Parameters[condition->ParameterID]->Value.TRIGGER;
									if (Parameters[condition->ParameterID]->Value.TRIGGER)
										Parameters[condition->ParameterID]->Value.TRIGGER = false;
									break;
								}
							}							
							break;
						}
						case AnimationCondition::ConditionOperation::GreaterThan:
						{
							switch (condition->ValueType)
							{
								case AnimationCondition::ConditionValueType::Float:
								{
									should_transition = should_transition && condition->Comparison_Value.FLOAT > Parameters[condition->ParameterID]->Value.FLOAT;
									break;
								}
								case AnimationCondition::ConditionValueType::Int:
								{
									should_transition = should_transition && condition->Comparison_Value.INTERGER > Parameters[condition->ParameterID]->Value.INTERGER;
									break;
								}
							}
							break;
						}
						case AnimationCondition::ConditionOperation::LessThan:
						{
							switch (condition->ValueType)
							{
							case AnimationCondition::ConditionValueType::Float:
							{
								should_transition = should_transition && condition->Comparison_Value.FLOAT < Parameters[condition->ParameterID]->Value.FLOAT;
								break;
							}
							case AnimationCondition::ConditionValueType::Int:
							{
								should_transition = should_transition && condition->Comparison_Value.INTERGER < Parameters[condition->ParameterID]->Value.INTERGER;
								break;
							}
							}
							break;
						}
					}
				}
				if (should_transition)
				{
					if (transition.second->FromState == current_state->ID)
					{
						Current_StateID = transition.second->ToState;
						States[Current_StateID]->Enabled = true;
						States[Current_StateID]->mTime = 0;
					}
				}
			}
		}
	}
	

	
	

}

void Fracture::AnimationGraph::SetCurrentState(Fracture::UUID state_id)
{
	Current_StateID = state_id;
	States[Current_StateID]->Enabled = true;
}

Fracture::AnimationState* Fracture::AnimationGraph::GetCurrentState()
{
	if (Current_StateID < 0)
		return nullptr;

	return States[Current_StateID].get();
}

void Fracture::AnimationGraph::TopologicalSort()
{
	adjList.clear();
	m_ExecutionOrder.clear();

	for (const auto& link : Links)
	{
		adjList[link.NodeFrom].push_back(link.NodeTo);
	}

	DepthFirstSearch();
	IsDirty = false;
}

void Fracture::AnimationGraph::Process(AnimationContext& context)
{
	for (const auto& node : m_ExecutionOrder)
	{
		node->Process(context);
	}
}

void Fracture::AnimationGraph::PushTask(std::shared_ptr<IAnimationTask> task)
{
	mAnimationTasks.push(task);
}

void Fracture::AnimationGraph::ExecuteTaskList()
{
	while(!mAnimationTasks.empty())
	{
		auto task = mAnimationTasks.front();
		task->Execute(*mContext.get());
		mAnimationTasks.pop();
	}
}

void Fracture::AnimationGraph::OnUpdate(float time)
{
	while (!mAnimationTasks.empty())
		mAnimationTasks.pop();

	if (EntityandMeshSet)
	{
		mContext->Time = time;
		if (IsDirty)
		{
			TopologicalSort();
		}

		Process(*mContext.get());

		ExecuteTaskList();

		CalculateFinalGlobalTransforms();
	}
}

void Fracture::AnimationGraph::CalculateFinalGlobalTransforms()
{
	auto mesh = AssetManager::GetStaticByIDMesh(mContext->MeshID);
	const auto& sampledBuffer = mSystem->mPool->GetPoseBuffer(0)->Pose;
	if (sampledBuffer.empty())
		return;
	mGlobalTansforms.resize(mesh->mBones.size());
	mLocalTansforms.resize(mesh->mBones.size());

	for (int i = 0; i < mesh->mBones.size(); i++)
	{
		auto& bone = mesh->mBones[mesh->mBoneOrder[i]];
		glm::mat4 parent_transform = glm::mat4(1.0f);

		if (i > 0) {
			parent_transform = mLocalTansforms[bone.ParentID];
		}

		{
			glm::mat4 m_translation = glm::translate(glm::mat4(1.0f), sampledBuffer[i].Position);
			glm::mat4 m_rotation = glm::toMat4(sampledBuffer[i].Rotation);
			glm::mat4 m_scale = glm::scale(glm::mat4(1.0f), sampledBuffer[i].Scale);
			bone.LocalTransformation = m_translation * m_rotation * m_scale;
		}

		mLocalTansforms[bone.ID] = parent_transform * bone.LocalTransformation;
		mGlobalTansforms[bone.ID] = mLocalTansforms[bone.ID] * mesh->mBones[bone.ID].InverseBindTransform;
	}
	mSystem->mPool->ReleasePoseBuffer(0);
}

std::vector<Fracture::PoseSample>& Fracture::AnimationGraph::GetPoseBufferAndSwap()
{
	auto& poses =  mPoseBuffer[mSwapBuffers];
	mSwapBuffers = !mSwapBuffers;
	GlobalIndex = (int)mSwapBuffers;
	return poses;
}

std::vector<Fracture::PoseSample>& Fracture::AnimationGraph::GetGlobalPoseBuffer(int index)
{	
	return mPoseBuffer[index];
}

void Fracture::AnimationGraph::SetPoseBuffer(std::vector<PoseSample> new_buffer)
{
	mPoseBuffer[0] = new_buffer;
	GlobalIndex = 0;
}

const std::shared_ptr<Fracture::IAnimationNode>& Fracture::AnimationGraph::GetNode(UUID node_id)
{
	auto it = std::find_if(std::begin(GraphNodes), std::end(GraphNodes), [&](std::shared_ptr<IAnimationNode> p) { return p->NodeID == node_id; });
	if (it != GraphNodes.end())
	{
		int index = std::distance(GraphNodes.begin(), it);
		return GraphNodes[index];
	}
	return nullptr;
}

void Fracture::AnimationGraph::DepthFirstSearch()
{
	for (const auto& node : GraphNodes)
	{
		node->isVisited = false;
	}


	std::queue<std::shared_ptr<IAnimationNode>> mstack;

	for (const auto& node : GraphNodes)
	{
		if (node->LinkCount == 0)
		{
			mstack.push(node);
		}
	}

	//Init Node Count
	int cnt = 0;

	while (!mstack.empty())
	{
		auto node = mstack.front();
		mstack.pop();
		m_ExecutionOrder.push_back(node);
	
		for (const auto& nodeID : adjList[node->NodeID])
		{
			auto adjNode = GetNode(nodeID);
			
			if (!adjNode)
				continue;

			adjNode->LinkCount--;
			if (adjNode->LinkCount == 0)
			{
				mstack.push(adjNode);
			}
		}
		cnt++;
	}

	int totalNodes = GraphNodes.size();
	if (cnt != totalNodes) {
		FRACTURE_CRITICAL("There exists a cycle in the graph please check your links \n");		
	}
	FRACTURE_TRACE("Sorted Graph");
	FRACTURE_TRACE("No of Nodes: {}", m_ExecutionOrder.size());

	for (const auto& node : m_ExecutionOrder)
	{
		FRACTURE_TRACE("Nodes: {}", node->Name);
	}

	//std::reverse(m_ExecutionOrder.begin(), m_ExecutionOrder.end());

	

}

