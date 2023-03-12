#pragma once
#ifndef RENDERGRAPHBUILDER_H
#define RENDERGRAPHBUILDER_H

#include "Rendergraph.h"

namespace Fracture
{
	struct RenderContext;

	struct RendergraphBuilder
	{
		RendergraphBuilder();

		static std::shared_ptr<Rendergraph> CreateGraph();

		static void ExecuteGraph(Rendergraph* graph);

		template<class T, typename... Args >
		static void AddPass(Rendergraph* graph,RenderContext* context,const std::string& name,Args&&... params);


		static void AddLink(Rendergraph* graph,const PassLink& link);
		static void BuildGraph(Rendergraph* graph);

		static void BuildAdjList(Rendergraph* graph);
		static void DFS(Rendergraph* graph);

	private:
		static std::vector<std::vector<std::string>> mAdjacencyLists;
	};


	template<class T,typename... Args >
	inline void RendergraphBuilder::AddPass(Rendergraph* graph,RenderContext* context,const std::string& name, Args&&... params)
	{
		auto pass = std::make_shared<T>(name,context,params...);
		graph->NameToPasses[name] = UUID();
		graph->Passes[name] = pass;	
	}

}


#endif