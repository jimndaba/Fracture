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

std::string& Fracture::RenderSink::GetName()
{
	return registeredName;
}

std::string& Fracture::RenderSink::GetSourceName()
{
	return sourceName;
}

std::string& Fracture::RenderSink::GetTargetPassName()
{
	return targetPass;
}

void Fracture::RenderSink::PostLinkValidate()
{
}
