#pragma once
#ifndef RENDERGRAPH_H
#define RENDERGRAPH_H


#include <string>
#include <vector>
#include <memory>

namespace Fracture
{

	class RenderPass;
	class RenderQueuePass;
	class RenderSource;
	class RenderSink;
	class Renderer;
	class RenderTarget;

	class RenderGraph
	{
	public:
		RenderGraph(Renderer& renderer);
		~RenderGraph();

		void Execute(Renderer& renderer);
		void Reset();

		void SetSinkTarget(const std::string& sinkName, const std::string& target);
		void AddGlobalSource(std::unique_ptr<RenderSource>);
		void AddGlobalSink(std::unique_ptr<RenderSink>);
		void Finalize();
		void AppendPass(std::unique_ptr<RenderPass> pass);
		RenderPass& FindPassByName(const std::string& name);
		std::shared_ptr<RenderTarget> backBufferTarget;
		//RenderQueuePass& GetRenderQueue(const std::string& passName);

	private:
		Renderer& m_Renderer;

		void LinkSinks(RenderPass& pass);
		void LinkGlobalSinks();

	private:
		std::vector<std::unique_ptr<RenderPass>> passes;
		std::vector<std::unique_ptr<RenderSource>> globalSources;
		std::vector<std::unique_ptr<RenderSink>> globalSinks;
		bool finalized = false;

	};

}

#endif