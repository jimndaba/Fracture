#pragma once
#ifndef FRAMEGRAPH_H
#define FRAMEGRAPH_H

#include <string>
#include <memory>
#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include "Link.h"
#include "FrameNode.h"
#include "SourceNode.h"
#include "Rendering/RenderTarget.h"
#include "PassLibrary/ClearFrame.h"
#include "PassLibrary/LambertianNode.h"
#include "PassLibrary/ToneMappingNode.h"
#include "PassLibrary/ThresholdNode.h"
#include "PassLibrary/AdditiveMixNode.h"
#include "PassLibrary/GaussianBlurNode.h"
#include "Rendering/Renderer.h"
#include "Profiling/Profiler.h"
#include "PassLibrary/DepthNode.h"
#include "PassLibrary/SSAONode.h"
#include "PassLibrary/MultiplyMix.h"
#include "PassLibrary/PickingPass.h"
#include "PassLibrary/IntermediateNode.h"
#include "PassLibrary/OutlineNode.h"
#include "PassLibrary/MixNode.h"
#include "PassLibrary/SSRNode.h"


namespace Fracture
{
	class FrameGraph
	{

	public:
		FrameGraph(Renderer& renderer);

		void addLink(const std::string& from,const std::string& source,const std::string& to,const std::string& resouce);	
		
		void addnode(std::shared_ptr<FrameNode> n);		

		std::shared_ptr<FrameNode> getNode(const std::string& name);

		void execute(Renderer& renderer);	

		void Buildgraph();		

		void DFSUtil(std::shared_ptr<FrameNode> v);
		
		void DFS(std::shared_ptr<FrameNode> v);

		void Resize(const int& width,const int& height);
		
		std::shared_ptr<SinkNode> GetOutput();

		std::vector<std::shared_ptr<FrameNode>> GetNodes();
		std::vector<std::shared_ptr<Link>> GetLinks();

		template<class T>
		static std::shared_ptr<T> Create(Renderer& renderer)
		{
			return std::make_shared<T>(renderer);
		}

	private:
		
		Renderer& m_Renderer;	
		//globals
		std::shared_ptr<RenderTarget> m_backBufferTarget;
		std::shared_ptr<SinkNode> outputbuffer;

		//Main collections
		std::vector<std::shared_ptr<FrameNode>> m_nodes;
		std::vector<std::shared_ptr<Link>> m_links;

		//DepthFirst Search and Sort
		std::map<std::string, std::vector<std::shared_ptr<FrameNode>>> adjList;			
		std::vector<std::shared_ptr<FrameNode>> m_FrameNodeQueue;

	};

}
#endif