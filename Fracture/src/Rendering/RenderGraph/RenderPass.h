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
		RenderSource& GetSource(std::string& name);
		RenderSink& GetSink(std::string& name);

		void RegisterSink(std::unique_ptr<RenderSink> sink);
		void RegisterSource(std::unique_ptr<RenderSink> source);

		void SetSinkLinkage(std::string& name, std::string& target);

	private:
		std::vector<std::unique_ptr<RenderSink>> m_Sinks;
		std::vector<std::unique_ptr<RenderSource>> m_Sources;		
		std::string name;
	};

	std::string SplitString(std::string& a, std::string& b)
	{
		std::string s = a;
		std::string delimiter = b;

		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);			
			s.erase(0, pos + delimiter.length());
		}		
	}
}

#endif