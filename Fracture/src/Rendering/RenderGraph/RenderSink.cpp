#include "RenderSink.h"

Fracture::RenderSink::RenderSink(std::string Name):registeredName(Name)
{
}

Fracture::RenderSink::~RenderSink()
{
}

void Fracture::RenderSink::SetTarget(std::string OutputSource, std::string TargetPass)
{
	sourceName = OutputSource;
	targetPass = TargetPass;
}

const std::string& Fracture::RenderSink::GetRegisteredName() const
{
	return registeredName;
}

const std::string& Fracture::RenderSink::GetSourceName() const
{
	return sourceName;
}

const std::string& Fracture::RenderSink::GetTargetPassName()const
{
	return targetPass;
}

void Fracture::RenderSink::PostLinkValidate()
{
}
