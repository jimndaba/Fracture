#include "RenderSource.h"
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

std::shared_ptr < Fracture:: Bindable > Fracture::RenderSource::YieldBindable()
{
	FRACTURE_ERROR("Output cannot be accessed as bindable");
	return nullptr;
}

std::shared_ptr<Fracture::BufferResource> Fracture::RenderSource::YieldBuffer()
{
	FRACTURE_ERROR("Output cannot be accessed as buffer");
	return nullptr;
}
