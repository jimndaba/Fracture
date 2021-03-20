#pragma once
#ifndef UIGRAPH_H
#define UIGRAPH_H

#include <string>
#include <memory>
#include <map>
#include <stack>
#include <vector>
#include <iostream>

#include "Rendering/DrawCommand.h"

namespace Fracture
{
	class Renderer;
	class Renderer2D;
	class UISocket;
	class UILink;
	class UINode;
	class UIResource;
	class UIOutputNode;
	class UIBoxNode;
	
	template<class T>
	class UIVector;

	class UIGraph
	{
	public:
		UIGraph(Renderer& renderer, Renderer2D& renderer2D);

		void addLink(const std::shared_ptr<UINode>& from, const UISocket& output, const std::shared_ptr<UINode>& to, const UISocket& input);

		void addnode(std::shared_ptr<UINode> n);

		std::shared_ptr<UINode> getNode(const std::string& name);

		void execute(Renderer& renderer);

		void Buildgraph();

		void DFSUtil(std::shared_ptr<UINode> v);

		void DFS(std::shared_ptr<UINode> v);

		//globals		
		std::shared_ptr<UIOutputNode> output;

		std::vector<std::shared_ptr<UINode>> GetNodes();
		std::vector<std::shared_ptr<UILink>> GetLinks();		

	private:

		Renderer& m_Renderer;
		Renderer2D& m_Renderer2D;

		//Main collections		
		std::vector<std::shared_ptr<UINode>> m_nodes;
		std::vector<std::shared_ptr<UILink>> m_links;

		//DepthFirst Search and Sort
		std::map<std::string, std::vector<std::shared_ptr<UINode>>> adjList;
		std::vector<std::shared_ptr<UINode>> m_FrameNodeQueue;


	};
}

#endif