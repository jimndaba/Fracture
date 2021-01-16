#include "RenderSource.h"
#include "BufferResource.h"
#include "Logging/Logger.h"

Fracture::RenderSource::RenderSource(std::string Name):name(Name)
{
}

Fracture::RenderSource::~RenderSource()
{
}

void Fracture::RenderSource::PostLinkValidate()
{
}

std::string& Fracture::RenderSource::GetName()
{
	return name;
}

std::shared_ptr<Fracture::BufferSource> Fracture::RenderSource::YieldBuffer()
{
	FRACTURE_ERROR("Output cannot be accessed as buffer");
	return;
}
