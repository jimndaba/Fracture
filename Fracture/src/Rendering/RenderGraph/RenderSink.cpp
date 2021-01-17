#include "RenderSink.h"
#include "Logging/Logger.h"

Fracture::RenderSink::RenderSink(std::string Name):registeredName(Name)
{
	if (registeredName.empty())
	{
		FRACTURE_ERROR("Empty output name");
	}
	const bool nameCharsValid = std::all_of(registeredName.begin(), registeredName.end(), [](char c) {
		return std::isalnum(c) || c == '_';
		});
	if (!nameCharsValid || std::isdigit(registeredName.front()))
	{
		FRACTURE_ERROR("Invalid output name: " + registeredName);
	}
}

Fracture::RenderSink::~RenderSink()
{
}

void Fracture::RenderSink::SetTarget(std::string passName, std::string outputName)
{
	{
		if (passName.empty())
		{
			FRACTURE_ERROR("Empty output name");
		}
		const bool nameCharsValid = std::all_of(passName.begin(), passName.end(), [](char c) {
			return std::isalnum(c) || c == '_';
			});
		if (passName != "$" && (!nameCharsValid || std::isdigit(passName.front())))
		{
			FRACTURE_ERROR("Invalid output name: " + registeredName);
		}
		this->passName = passName;
	}
	{
		if (outputName.empty())
		{
			FRACTURE_ERROR("Empty output name");
		}
		const bool nameCharsValid = std::all_of(outputName.begin(), outputName.end(), [](char c) {
			return std::isalnum(c) || c == '_';
			});
		if (!nameCharsValid || std::isdigit(outputName.front()))
		{
			FRACTURE_ERROR("Invalid output name: " + registeredName);
		}
		this->outputName = outputName;
	}
}

const std::string& Fracture::RenderSink::GetRegisteredName() const
{
	return registeredName;
}

const std::string& Fracture::RenderSink::GetSourceName() const
{
	return outputName;
}

const std::string& Fracture::RenderSink::GetTargetPassName()const
{
	return passName;
}

void Fracture::RenderSink::PostLinkValidate()
{
}
