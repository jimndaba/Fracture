#include "FracturePCH.h"
#include "RenderGraphBuilder.h"

std::vector<std::vector<std::string>> Fracture::RendergraphBuilder::mAdjacencyLists;

Fracture::RendergraphBuilder::RendergraphBuilder()
{
}

std::shared_ptr<Fracture::Rendergraph> Fracture::RendergraphBuilder::CreateGraph()
{
    return std::make_shared<Fracture::Rendergraph>();
}

void Fracture::RendergraphBuilder::ExecuteGraph(Rendergraph* graph)
{
	for (const auto& pass : graph->PassQueue)
	{
		graph->Passes[pass]->Execute();
	}
}

void Fracture::RendergraphBuilder::AddLink(Rendergraph* graph, const PassLink& link)
{
    graph->PassLinks.push_back(link);
}

void Fracture::RendergraphBuilder::BuildGraph(Rendergraph* graph)
{
	BuildAdjList(graph);
	DFS(graph);
}

void Fracture::RendergraphBuilder::BuildAdjList(Rendergraph* graph)
{
	mAdjacencyLists.resize(graph->Passes.size());

	int PassIndex = 0;
	int OtherPassIndex = 0;

	for (const auto& pass : graph->Passes)
	{
		std::vector<std::string>& adjacentNodeIndices = mAdjacencyLists[PassIndex];
	
		for (const auto& other : graph->Passes)
		{
			
			// Do not check dependencies on itself
			if (pass.second->Name == other.second->Name) continue;

			const auto& other_pass = other.second;
			int link_index = 0;

			for (const auto& link : graph->PassLinks)
			{
				bool otherNodeDependsOnCurrentNode = (link.To == other_pass->Name);
				
				if (otherNodeDependsOnCurrentNode)
				{
					adjacentNodeIndices.push_back(other_pass->Name);
					std::vector<PassLink>::iterator it = graph->PassLinks.begin();
					std::advance(it, link_index);
					graph->PassLinks.erase(it);
					continue;
				}
				link_index++;
			}
			OtherPassIndex++;
		
		}
		PassIndex++;
	}	

	FRACTURE_INFO("Adjesency list: {}", mAdjacencyLists.size());
}

void Fracture::RendergraphBuilder::DFS(Rendergraph* graph)
{
	auto NoOfPasses = graph->Passes.size();
	std::vector<bool> visited(NoOfPasses, false);
	std::vector<bool> onStack(NoOfPasses, false);
	std::vector<int> onPassIndexes(NoOfPasses);
	bool isCyclic = false;
	std::queue<std::string> mstack;


	int pass_index = 0;
	for (const auto& pass : graph->Passes)
	{
		if (mAdjacencyLists[pass_index].empty())
		{
			visited[pass_index] = true;
			onStack[pass_index] = true;
			mstack.push(pass.second->Name);
		}
		pass_index++;
	}

	pass_index = 0;
	while (!mstack.empty())
	{
		if (isCyclic) return;

		auto PassName = mstack.front();		
		graph->PassQueue.push_back(PassName);
		mstack.pop();

		for (const auto& pass : graph->Passes)
		{
			if (visited[pass_index] && onStack[pass_index])
			{
				isCyclic = true;
				return;
			}
			if (!visited[pass_index])
			{		
				visited[pass_index] = true;
				mstack.push(pass.first);
			}
			onStack[pass_index] = false;
			pass_index++;
		}
	}


	for (int i = 0; i < graph->PassQueue.size(); i++)
	{
		auto name = graph->PassQueue[i];
		FRACTURE_INFO("Render Graph Order: {}", graph->PassQueue[i]);
	}

	/*
	std::queue<std::shared_ptr<IPass>> mstack;

	for (const auto& node : graph->Passes)
	{

		mstack.push(node.second);
	}

	//Init Node Count
	int cnt = 0;

	while (!mstack.empty())
	{
		auto node = mstack.front();
		mstack.pop();
		m_FrameNodeQueue.push_back(node);
		
		std::string name = node->Name;
		UUID node_id = node->ID;
		for (const auto& linkstring : adjList[node_id])
		{
			auto adjNode = GetNodebyID(linkstring);
			adjNode->RemoveLink();
			
			
			if (adjNode->LinkCount() == 0)
			{
				mstack.push(adjNode);
			}
		}
		cnt++;

	}
	*/
}

