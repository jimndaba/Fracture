#include "RenderGraph.h"
#include "Rendering/Renderer.h"
#include "Passes/RenderPass.h"
#include "Logging/Logger.h"
#include "DirectBufferSink.h"
#include "RenderGraphUtils.h"
#include <sstream>

Fracture::RenderGraph::RenderGraph(Renderer& renderer):m_Renderer(renderer),backBufferTarget(renderer.SceneRenderTarget)
{
	AddGlobalSource(DirectBufferSource<RenderTarget>::Make("backbuffer", backBufferTarget));
	AddGlobalSink(DirectBufferSink<RenderTarget>::Make("backbuffer", backBufferTarget));
}

Fracture::RenderGraph::~RenderGraph()
{
}

void Fracture::RenderGraph::Execute(Renderer& renderer)
{
	if (finalized)
	{
		for (auto& p : passes)
		{
			p->Execute(renderer);
		}
	}

}

void Fracture::RenderGraph::Reset()
{
	if (finalized)
	{
		for (auto& p : passes)
		{
			p->Reset();
		}
	}
}

void Fracture::RenderGraph::SetSinkTarget(const std::string& sinkName, const std::string& target)
{
	const auto finder = [&sinkName](const std::unique_ptr<RenderSink>& p) {
		return p->GetRegisteredName() == sinkName;};

	const auto i = std::find_if(globalSinks.begin(), globalSinks.end(), finder);
	if (i == globalSinks.end())
	{
		FRACTURE_ERROR("Global sink does not exist: " + sinkName);
	}
	auto targetSplit = SplitString(target, ".");
	if (targetSplit.size() != 2u)
	{
		FRACTURE_ERROR("Input target has incorrect format");
	}

	(*i)->SetTarget(targetSplit[0], targetSplit[1]);

}

void Fracture::RenderGraph::AddGlobalSource(std::unique_ptr<RenderSource> out)
{
	globalSources.push_back(std::move(out));
}

void Fracture::RenderGraph::AddGlobalSink(std::unique_ptr<RenderSink> in)
{
	globalSinks.push_back(std::move(in));
}

void Fracture::RenderGraph::Finalize()
{
	if (!finalized)
	{
		for (const auto& p : passes)
		{
			p->Finalise();
		}
		LinkGlobalSinks();
		finalized = true;
	}	
}

void Fracture::RenderGraph::AppendPass(std::unique_ptr<RenderPass> pass)
{
	assert(!finalized);
	// validate name uniqueness
	for (const auto& p : passes)
	{
		if (pass->GetName() == p->GetName())
		{
			FRACTURE_ERROR("Pass name already exists: " + pass->GetName());
		}
	}

	// link outputs from passes (and global outputs) to pass inputs
	LinkSinks(*pass);

	// add to container of passes
	passes.push_back(std::move(pass));
}

Fracture::RenderPass& Fracture::RenderGraph::FindPassByName(const std::string& name)
{
	const auto i = std::find_if(passes.begin(), passes.end(), [&name](auto& p) {
		return p->GetName() == name;
		});
	if (i == passes.end())
	{
		FRACTURE_ERROR("Failed to find pass name");
	}
	return **i;
}

void Fracture::RenderGraph::LinkSinks(RenderPass& pass)
{
	for (auto& si : pass.GetSinks())
	{
		const auto& inputSourcePassName = si->GetTargetPassName();

		if (inputSourcePassName.empty())
		{
			std::ostringstream oss;
			FRACTURE_ERROR("In pass named ["+pass.GetName() + "] sink named [" +si->GetRegisteredName() + "] has no target source set.");
			//throw RGC_EXCEPTION(oss.str());
		}

		// check check whether target source is global
		if (inputSourcePassName == "$")
		{
			bool bound = false;
			for (auto& source : globalSources)
			{
				if (source->GetName() == si->GetSourceName())
				{
					si->Bind(*source);
					bound = true;
					break;
				}
			}
			if (!bound)
			{
				std::ostringstream oss;
				FRACTURE_ERROR("Output named [" + si->GetSourceName()+ "] not found in globals");
				//throw RGC_EXCEPTION(oss.str());
			}
		}
		else // find source from within existing passes
		{
			bool bound = false;
			for (auto& existingPass : passes)
			{
				if (existingPass->GetName() == inputSourcePassName)
				{
					auto& source = existingPass->GetSource(si->GetSourceName());
					si->Bind(source);
					bound = true;
					break;
				}
			}
			if (!bound)
			{
				std::ostringstream oss;
				FRACTURE_ERROR( "Pass named ["+ inputSourcePassName + "] not found");
				//throw RGC_EXCEPTION(oss.str());
			}
		}
	}
}

void Fracture::RenderGraph::LinkGlobalSinks()
{
	for (auto& sink : globalSinks)
	{
		const auto& inputSourcePassName = sink->GetTargetPassName();
		for (auto& existingPass : passes)
		{
			if (existingPass->GetName() == inputSourcePassName)
			{
				auto& source = existingPass->GetSource(sink->GetSourceName());
				sink->Bind(source);
				break;
			}
		}
	}
}
