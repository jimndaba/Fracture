#include "RenderPass.h"
#include "RenderSink.h"
#include "RenderSource.h"
#include "Rendering/Renderer.h"
#include "Logging/Logger.h"


Fracture::RenderPass::RenderPass(std::string Name):name(Name)
{
}

Fracture::RenderPass::~RenderPass()
{
}

void Fracture::RenderPass::Finalise()
{
	for (auto& in : m_Sinks)
	{
		in->PostLinkValidate();
	}
	for (auto& out : m_Sources)
	{
		out->PostLinkValidate();
	}
}

void Fracture::RenderPass::Reset()
{
}

std::string& Fracture::RenderPass::GetName()
{
	return name;
}

Fracture::RenderSource& Fracture::RenderPass::GetSource(std::string& name)
{
	for (auto& source : m_Sources)
	{
		if (source->GetName() == name)
		{
			return *source;
		}
	}
	FRACTURE_ERROR("Could Not Find RenderSource " + name);
	return;
}

Fracture::RenderSink& Fracture::RenderPass::GetSink(std::string& name)
{
	for (auto& sink : m_Sinks)
	{
		if (sink->GetName() == name)
		{
			return *sink;
		}
	}
	FRACTURE_ERROR("Could Not Find RenderSink " + name);
	return;
}

void Fracture::RenderPass::RegisterSink(std::unique_ptr<RenderSink> sink)
{
	for (auto& sk : m_Sinks)
	{
		if (sk->GetName() == sink->GetName())
		{
			FRACTURE_ERROR("Sink Alread Registered!" + sink->GetName());
			return;
		}
	}
	m_Sinks.push_back(std::move(sink));
}

void Fracture::RenderPass::RegisterSource(std::unique_ptr<RenderSink> source)
{
	for (auto& src : m_Sources)
	{
		if (src->GetName() == source->GetName())
		{
			FRACTURE_ERROR("Sink Alread Registered!" + source->GetName());
			return;
		}
	}
	m_Sinks.push_back(std::move(source));
}

void Fracture::RenderPass::SetSinkLinkage(std::string& name, std::string& target)
{

}

