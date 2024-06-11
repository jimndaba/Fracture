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

}

void Fracture::AnimationGraph::SetCurrentState(Fracture::UUID state_id)
{
	Current_StateID = state_id;	
}

Fracture::AnimationState* Fracture::AnimationGraph::GetCurrentState()
{
	if (Current_StateID < 0)
		return nullptr;

	return nullptr;
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

void Fracture::AnimationGraph::OnUpdate(float time,Fracture::UUID entityid)
{
	while (!mAnimationTasks.empty())
		mAnimationTasks.pop();

	mContext->EntityID = entityid;
	mContext->Time = time;


	if (IsDirty)
	{
		TopologicalSort();
	}

	Process(*mContext.get());

	ExecuteTaskList();

	CalculateFinalGlobalTransforms();

	mSystem->mGlobalPoseMatrices[entityid] = mGlobalTansforms;
}

void Fracture::AnimationGraph::CalculateFinalGlobalTransforms()
{
	auto mesh = AssetManager::GetStaticByIDMesh(mContext->MeshID);
	const auto& sampledBuffer = mSystem->mPool->GetPoseBuffer(0)->Pose;
	if (sampledBuffer.empty())
		return;
	mGlobalTansforms.resize(mesh->mBones.size());
	mLocalTansforms.resize(mesh->mBones.size());
	
	if (mContext->IsPlaying)
	{
		for (int i = 0; i < mesh->mBones.size(); i++)
		{

			glm::mat4 m_translation = glm::translate(glm::mat4(1.0f), sampledBuffer[i].Position);
			glm::mat4 m_rotation = glm::toMat4(sampledBuffer[i].Rotation);
			glm::mat4 m_scale = glm::scale(glm::mat4(1.0f), sampledBuffer[i].Scale);

			auto& bone = mesh->mBones[mesh->mBoneOrder[i]];//
			glm::mat4 parent_transform = glm::mat4(1.0f);
			if (i != 0)
			{
				parent_transform = mLocalTansforms[bone.ParentID];
			}

			bone.LocalTransformation = m_translation * m_rotation * m_scale;
			mLocalTansforms[bone.ID] = parent_transform * bone.LocalTransformation;
			mGlobalTansforms[bone.ID] = bone.SceneRootTransform * mLocalTansforms[bone.ID] * mesh->mBones[bone.ID].InverseBindTransform;
		}
	}
	else
	{
		for (int i = 0; i < mesh->mBones.size(); i++)
		{		
			auto& bone = mesh->mBones[mesh->mBoneOrder[i]];//mesh->mBoneOrder[i]
			glm::mat4 parent_transform = glm::mat4(1.0f);
			if (i != 0)
			{
				parent_transform = mLocalTansforms[bone.ParentID];
			}

			mLocalTansforms[bone.ID] = parent_transform * bone.LocalTransformation;
			mGlobalTansforms[bone.ID] = mLocalTansforms[bone.ID] * mesh->mBones[bone.ID].InverseBindTransform;
		}		
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

Fracture::IAnimationNode* Fracture::AnimationGraph::GetNode(UUID node_id)
{
	auto it = std::find_if(std::begin(GraphNodes), std::end(GraphNodes), [node_id](const std::unique_ptr<IAnimationNode>& p) { return p->NodeID == node_id; });
	if (it != GraphNodes.end())
	{
		int index = std::distance(GraphNodes.begin(), it);
		return GraphNodes[index].get();
	}
	else
	{
		FRACTURE_ERROR("Could not find node: {} ", node_id);
	}
	return nullptr;
}

void Fracture::AnimationGraph::TopologicalSort()
{
	adjList.clear();
	m_ExecutionOrder.clear();

	for (const auto& node : GraphNodes)
	{
		node->LinkCount  = 0;
	}


	for (const auto& link : Links)
	{
		adjList[link.NodeFrom].push_back(link.NodeTo);

		const auto& node = GetNode(link.NodeTo);
		node->LinkCount += 1;
	}



	DepthFirstSearch();
	IsDirty = false;
}

void Fracture::AnimationGraph::DepthFirstSearch()
{
	for (const auto& node : GraphNodes)
	{
		node->isVisited = false;
	}


	std::queue<IAnimationNode*> mstack;

	for (const auto& node : GraphNodes)
	{
		if (node->LinkCount == 0)
		{
			mstack.push(node.get());
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

			adjNode->LinkCount -= 1;
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

