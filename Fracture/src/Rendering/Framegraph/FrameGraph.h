#pragma once
#ifndef FRAMEGRAPH_H
#define FRAMEGRAPH_H

#include <string>
#include <memory>
#include <map>
#include <stack>
#include <vector>
#include <iostream>

namespace Fracture
{

	class FrameNode;
	class Link;
	class Renderer;
	class RenderTarget;
	class ToneMappingNode;
	class ThresholdNode;
	class BoxBlurNode;
	class SinkNode;
	class SSAONode;

	class FrameGraph
	{

	public:
		FrameGraph(Renderer& renderer);

		void addLink(std::string from, std::string source, std::string to, std::string resouce);	
		void addGlobalLink(std::string to, std::string resouce);
		void linkGlobalResources();
		void addnode(std::shared_ptr<FrameNode> n);		

		std::shared_ptr<FrameNode> getNode(std::string name);

		void execute(Renderer& renderer);
		

		void Buildgraph();
		

		void DFSUtil(std::shared_ptr<FrameNode> v);
		
		void DFS(std::shared_ptr<FrameNode> v);

		void Resize(int width, int height);
		

		std::shared_ptr<ToneMappingNode> ToneMap;
		std::shared_ptr<ThresholdNode> BrightPass;
		std::shared_ptr<SSAONode> ssao;
		std::shared_ptr<BoxBlurNode> ssaoblur;
		std::shared_ptr<SinkNode> outputbuffer;
	private:
		//std::shared_ptr<InputSourceNode> resultsource;
		Renderer& m_Renderer;	
		std::shared_ptr<RenderTarget> m_backBufferTarget;
		std::vector<std::shared_ptr<Link>> m_links;
		//std::vector<Link> m_globallinks;
		std::map<std::string, std::vector<std::shared_ptr<FrameNode>>> adjList;
		std::vector<std::shared_ptr<FrameNode>> m_nodes;		
		std::vector<std::shared_ptr<FrameNode>> m_FrameNodeQueue;

	};

}
#endif