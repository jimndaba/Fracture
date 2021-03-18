#include "FrameGraph.h"

Fracture::FrameGraph::FrameGraph(Renderer& renderer) :m_Renderer(renderer), m_backBufferTarget(renderer.SceneRenderTarget)
{
	{
		auto backbuffer = std::make_shared<SourceNode>("global_backbuffer", m_backBufferTarget);
		addnode(backbuffer);
	}

	{
		outputbuffer = std::make_shared<SinkNode>("global_output", renderer.Width(), renderer.Height());
		addnode(outputbuffer);
	}

}

void Fracture::FrameGraph::addLink(const std::string& from, const std::string& source, const std::string& to, const std::string& resouce)
{
	std::shared_ptr<Link> mlink = std::make_shared<Link>();
	mlink->FrameNode_From= from;
	mlink->FrameNode_To = to;
	mlink->FrameNode_Resource= resouce;
	mlink->FrameNode_Source = source;
	m_links.push_back(mlink);
}

void Fracture::FrameGraph::addnode(std::shared_ptr<FrameNode> n)
{
	m_nodes.push_back(n);
}

std::shared_ptr<Fracture::FrameNode> Fracture::FrameGraph::getNode(const std::string& name)
{
	for (auto& n : m_nodes)
	{
		if (n->GetName() == name)
		{
			return n;
		}
	}
}

void Fracture::FrameGraph::execute(Renderer& renderer)
{
	ProfilerTimer timer("Execute Framegraph");
	for (const auto& node : m_FrameNodeQueue)
	{
		for (auto& m : m_links)
		{
			if (m->FrameNode_From == node->GetName())
			{				
				for (const auto& n : m_nodes)
				{
					if (n->GetName() == m->FrameNode_To)
					{
						std::shared_ptr<FrameResource> resource = n->getResource(m->FrameNode_Resource);
						node->LinkResource(m->FrameNode_Source, resource);
					}
				}
			}
		}
		node->execute(renderer);
	}
}

void Fracture::FrameGraph::Buildgraph()
{
	// add edges to the directed graph
	for (auto& link : m_links)
	{
		// insert at the end
		adjList[link->FrameNode_From].push_back(getNode(link->FrameNode_To));	
	}

	DFS(outputbuffer);
	//DFS(adjList["global_output"][0]);
}

void Fracture::FrameGraph::DFSUtil(std::shared_ptr<FrameNode> v)
{
	std::stack<std::shared_ptr<FrameNode>> mstack;
	mstack.push(v);

	while (!mstack.empty())
	{
		v = mstack.top();
		mstack.pop();

		if (!v->isVisited)
		{			
			m_FrameNodeQueue.push_back(v);
			v->isVisited = true;
		}

		for (const auto& adjNode : adjList[v->GetName()])
		{
			if (!adjNode->isVisited)
			{
				mstack.push(adjNode);			
			}
		}

	}

	std::reverse(m_FrameNodeQueue.begin(), m_FrameNodeQueue.end());
}

void Fracture::FrameGraph::DFS(std::shared_ptr<FrameNode> v)
{

	for (int i = 0; i < m_nodes.size(); i++)
		m_nodes[i]->isVisited = false;

	DFSUtil(v);
}

void Fracture::FrameGraph::Resize(const int& width,const int& height)
{
	for (auto& node : m_nodes)
	{
		node->Resize(width, height);
	}
}

std::shared_ptr<Fracture::SinkNode> Fracture::FrameGraph::GetOutput()
{
	return outputbuffer;
}

std::vector<std::shared_ptr<Fracture::FrameNode>> Fracture::FrameGraph::GetNodes()
{
	return m_nodes;
}

std::vector<std::shared_ptr<Fracture::Link>> Fracture::FrameGraph::GetLinks()
{
	return m_links;
}
