#include "FrameGraph.h"
#include "Link.h"
#include "FrameNode.h"
#include "SourceNode.h"
#include "Rendering/RenderTarget.h"
#include "PassLibrary/ClearFrame.h"
#include "PassLibrary/LambertianNode.h"
#include "PassLibrary/InvertNode.h"
#include "PassLibrary/ToneMappingNode.h"
#include "PassLibrary/ThresholdNode.h"
#include "PassLibrary/AdditiveMixNode.h"
#include "PassLibrary/BoxBlurNode.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"
#include "PassLibrary/DepthNode.h"
#include "PassLibrary/SSAONode.h"
#include "PassLibrary/MultiplyMix.h"
#include "PassLibrary/PickingPass.h"
#include "PassLibrary/IntermediateNode.h"

Fracture::FrameGraph::FrameGraph(Renderer& renderer) :m_Renderer(renderer), m_backBufferTarget(renderer.SceneRenderTarget)
{
	{
		auto backbuffer = std::make_shared<SourceNode>("global_backbuffer", m_backBufferTarget);
		addnode(backbuffer);
	}	

	{
		auto depthbuffer = std::make_shared<DepthNode>("global_depthbuffer",renderer.Width(), renderer.Height(), renderer.m_Bucket.get());
		addnode(depthbuffer);
	}

	{
		auto clear = std::make_shared<ClearFrame>("clearframe");
		addnode(clear);
	}


	{
		auto lambertian = std::make_shared<LambertianNode>("lamertianPass", renderer.Width(), renderer.Height(), renderer.m_Bucket.get());
		addnode(lambertian);
	}
	{
		BrightPass = std::make_shared<ThresholdNode>("thresholdPass",renderer.Width(),renderer.Height());
		addnode(BrightPass);
	}

	
		ToneMap = std::make_shared<ToneMappingNode>("ToneMapPass", renderer.Width(), renderer.Height());
		addnode(ToneMap);
	

	{
	    auto blurPass = std::make_shared<BoxBlurNode>("BoxBlurPass", renderer.Width(), renderer.Height());
		addnode(blurPass);
	}

	{
		auto mixColor = std::make_shared<AdditiveMixNode>("mixPass", renderer.Width(), renderer.Height());
		addnode(mixColor);
	}


	{
		ssao = std::make_shared<SSAONode>("ssaoPass", renderer.Width(), renderer.Height());
		addnode(ssao);
	}

	{
		ssaoblur = std::make_shared<BoxBlurNode>("ssaoBlur", renderer.Width(), renderer.Height());
		addnode(ssaoblur);
	}


	{
		auto intermediate = std::make_shared<IntermediateNode>("intermediatePass", renderer.Width(), renderer.Height());
		addnode(intermediate);
	}



	{
		outputbuffer = std::make_shared<SinkNode>("global_output", renderer.Width(), renderer.Height());
		addnode(outputbuffer);
	}


	addLink("global_output", "rendertarget", "mixPass", "output");

	

	addLink("mixPass", "colorA", "intermediatePass", "OutTexture");
	addLink("mixPass", "colorB", "BoxBlurPass", "blurOutput");
	


	addLink("BoxBlurPass", "colorTexture", "thresholdPass", "thresholdMap");
	
	addLink("thresholdPass", "colorTexture", "ToneMapPass", "colorOut");
	
	addLink("ToneMapPass", "buffer", "intermediatePass", "OutTexture");
		
	//addLink("multiplyPass", "colorA", "lamertianPass", "outputColor");
	//addLink("multiplyPass", "colorB", "ssaoBlur", "blurOutput");
	
	addLink("intermediatePass", "inputbuffer", "lamertianPass",  "outputColor");

	addLink("lamertianPass", "buffer", "clearframe", "buffer");
	addLink("lamertianPass", "SSAOMap", "ssaoBlur", "blurOutput");



	addLink("clearframe", "buffer","global_backbuffer", "rendertarget");
	
	
	addLink("ssaoBlur", "colorTexture", "ssaoPass", "SSAOOutput");

	addLink("ssaoPass", "DepthTexture", "global_depthbuffer", "outputDepthMap");

}

void Fracture::FrameGraph::addLink(std::string from, std::string source, std::string to, std::string resouce)
{
	std::shared_ptr<Link> mlink = std::make_shared<Link>();
	mlink->FrameNode_From= from;
	mlink->FrameNode_To = to;
	mlink->FrameNode_Resource= resouce;
	mlink->FrameNode_Source = source;
	m_links.push_back(mlink);
}

void Fracture::FrameGraph::addGlobalLink(std::string to, std::string resouce)
{

}

void Fracture::FrameGraph::linkGlobalResources()
{

}

void Fracture::FrameGraph::addnode(std::shared_ptr<FrameNode> n)
{
	m_nodes.push_back(n);
}

std::shared_ptr<Fracture::FrameNode> Fracture::FrameGraph::getNode(std::string name)
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

void Fracture::FrameGraph::Resize(int width,int height)
{
	for (auto& node : m_nodes)
	{
		node->Resize(width, height);
	}

}