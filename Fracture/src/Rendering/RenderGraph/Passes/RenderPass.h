#ifndef RENDERPASS_H
#define RENDERPASS_H

#include <vector>
#include <memory>
#include <string>

namespace Fracture
{
	class Renderer;
	class RenderSink;
	class RenderSource;

	class RenderPass
	{

	public:
		RenderPass(std::string Name);
		virtual ~RenderPass();
		virtual void Execute(Renderer& renderer) = 0;
		virtual void Finalise();
		virtual void Reset();

		//Sink and Source Getters
		std::string& GetName();
		RenderSource& GetSource(const std::string& name);
		RenderSink& GetSink(const std::string& name);

		void RegisterSink(std::unique_ptr<RenderSink> sink);
		void RegisterSource(std::unique_ptr<RenderSource> source);
		void SetSinkLinkage(const std::string& name,const  std::string& target);

		const std::vector<std::unique_ptr<RenderSink>>& GetSinks() const;

	private:
		std::vector<std::unique_ptr<RenderSink>> m_Sinks;
		std::vector<std::unique_ptr<RenderSource>> m_Sources;		
		std::string name;
	};
	
}

#endif