#include "UIGraph.h"
#include "Rendering/Renderer.h"
#include "UINode.h"
#include "UILink.h"
#include "UISocket.h"
#include "UINode.h"
#include "UIResource/UIResource.h"
#include "UIResource/UICollections.h"
#include "NodeLibrary/UIOutputNode.h"
#include "NodeLibrary/UIBoxNode.h"
#include "NodeLibrary/UISectorNode.h"
#include "NodeLibrary/UISetPositionNode.h"
#include "NodeLibrary/UILoopNode.h"
#include "UIResource/UIFunction.h"
#include "NodeLibrary/UILabelNode.h"
#include "Profiling/Profiler.h"

Fracture::UIGraph::UIGraph(Renderer& renderer, Renderer2D& renderer2D):
	m_Renderer(renderer),
	m_Renderer2D(renderer2D),
	output(std::make_shared<UIOutputNode>(renderer2D,"Output",renderer.Width(),renderer.Height()))
{	
	addnode(output);	
	output->Submit.AddChildSocket("Box1");
	output->Submit.AddChildSocket("Box2");
	output->Submit.AddChildSocket("Box3");
	output->Submit.AddChildSocket("Circle");
	output->Submit2D.AddChildSocket("Label");
	
	auto box = UINode::Make<UIBoxNode>("Box");
	addnode(box);

	auto circle = UINode::Make<UISectorNode>("Circle");
	addnode(circle);

	auto label = UINode::Make<UILabelNode>("Label");
	label->SetText("FRACTURE ENGINE TEXT");
	addnode(label);

	auto position = UINode::Make<UISetPositionNode>("box2pos");
	position->SetPosition(glm::vec3(5.0f,1.0f,0.0f));
	addnode(position);
	
	auto box2 = UINode::Make<UIBoxNode>("Box2");
	addnode(box2);

	auto box3 = UINode::Make<UIBoxNode>("Box3");
	addnode(box3);

	

	auto position3 = UINode::Make<UISetPositionNode>("box3pos");
	position3->SetPosition(glm::vec3(2.0f, 4.0f, 1.0f));
	addnode(position3);


	addLink(circle, circle->PositionSocket, position3, position3->Output);	
	addLink(box2, box2->PositionSocket, position, position->Output);
	
	addLink(output, output->Submit.GetSocket("Circle"), circle, circle->Exectue);
	addLink(output, output->Submit.GetSocket("Box3"), box3, box3->Exectue);	
	addLink(output, output->Submit.GetSocket("Box1"), box2, box2->Exectue);
	addLink(output, output->Submit.GetSocket("Box2"), box, box->Exectue);
	addLink(output, output->Submit2D.GetSocket("Label"), label, label->Exectue);

	
}

void Fracture::UIGraph::addLink(const std::shared_ptr<UINode>& from, const UISocket& output, const std::shared_ptr<UINode>& to, const UISocket& input)
{
	std::shared_ptr<UILink> mlink = std::make_shared<UILink>();
	mlink->From = from->GetName();
	mlink->To = to->GetName();
	mlink->FromResource = input.GetName();
	mlink->ToSource = output.GetName();
	m_links.push_back(mlink);
}

void Fracture::UIGraph::addnode(std::shared_ptr<UINode> n)
{
	m_nodes.push_back(n);
}

std::shared_ptr<Fracture::UINode> Fracture::UIGraph::getNode(const std::string& name)
{
	for (auto& n : m_nodes)
	{
		if (n->GetName() == name)
		{
			return n;
		}
	}
}

void Fracture::UIGraph::execute(Renderer& renderer)
{
	ProfilerTimer timer("Execute UIGraph");
	for (const auto& node : m_FrameNodeQueue)
	{
		for (auto& m : m_links)
		{
			if (m->From == node->GetName())
			{
				const auto& ToNode = getNode(m->To);
				std::shared_ptr<UIResource> resource =ToNode->getResource(m->FromResource);
				node->LinkUIResource(m->ToSource, resource);
			}	
		}		
		node->execute(renderer);
	}	
}

void Fracture::UIGraph::Buildgraph()
{
	// add edges to the directed graph
	for (auto& link : m_links)
	{
		// insert at the end
		adjList[link->From].push_back(getNode(link->To));
	}

	DFS(output); //--- START POINT
}

void Fracture::UIGraph::DFSUtil(std::shared_ptr<UINode> v)
{
	std::stack<std::shared_ptr<UINode>> mstack;
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

void Fracture::UIGraph::DFS(std::shared_ptr<UINode> v)
{
	for (int i = 0; i < m_nodes.size(); i++)
		m_nodes[i]->isVisited = false;

	DFSUtil(v);
}

std::vector<std::shared_ptr<Fracture::UINode>> Fracture::UIGraph::GetNodes()
{
    return m_nodes;
}

std::vector<std::shared_ptr<Fracture::UILink>> Fracture::UIGraph::GetLinks()
{
    return m_links;
}

