#include "RenderPass.h"
#include "../RenderSink.h"
#include "../RenderSource.h"
#include "../RenderGraphUtils.h"
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

Fracture::RenderSource& Fracture::RenderPass::GetSource(const std::string& name)
{
	for (auto& source : m_Sources)
	{
		if (source->GetName() == name)
		{
			return *source;
		}
	}
	FRACTURE_ERROR("Could Not Find RenderSource " + name);	
}

Fracture::RenderSink& Fracture::RenderPass::GetSink(const std::string& name)
{
	for (auto& sink : m_Sinks)
	{
		if (sink->GetRegisteredName() == name)
		{
			return *sink;
		}
	}	
	FRACTURE_ERROR("Could Not Find Sink " + name);
}

void Fracture::RenderPass::RegisterSink(std::unique_ptr<RenderSink> sink)
{
	for (auto& sk : m_Sinks)
	{
		if (sk->GetRegisteredName() == sink->GetRegisteredName())
		{
			FRACTURE_ERROR("Sink Alread Registered!" + sink->GetRegisteredName());
			return;
		}
	}
	m_Sinks.push_back(std::move(sink));
}

void Fracture::RenderPass::RegisterSource(std::unique_ptr<RenderSource> source)
{
	for (auto& src : m_Sources)
	{
		if (src->GetName() == source->GetName())
		{
			FRACTURE_ERROR("Source Alread Registered!" + source->GetName());
			return;
		}
	}
	m_Sources.push_back(std::move(source));
}

void Fracture::RenderPass::SetSinkLinkage(const std::string& name, const  std::string& target)
{
	auto& sink = GetSink(name);
	auto targetSplit = SplitString(target, ".");
	if (targetSplit.size() != 2u)
	{
		FRACTURE_ERROR("Input target has incorrect format");
	}
	sink.SetTarget(std::move(targetSplit[0]), std::move(targetSplit[1]));

}

const std::vector<std::unique_ptr<Fracture::RenderSink>>& Fracture::RenderPass::GetSinks() const
{
	return m_Sinks;
}

