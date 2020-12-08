#include "MaterialFactory.h"

Fracture::MaterialFactory::MaterialFactory()
{
}

Fracture::MaterialFactory::~MaterialFactory()
{
}

std::shared_ptr <Fracture::Material > Fracture::MaterialFactory::OpaqueMaterial()
{
	return std::shared_ptr<Material>();
}

std::shared_ptr<Fracture::Material> Fracture::MaterialFactory::PBRMaterial()
{
	return std::shared_ptr<Material>();
}
