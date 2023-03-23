#include "FracturePCH.h"
#include "MaterialLoader.h"
#include "Serialisation/Serialiser.h"
#include "Serialisation/MaterialSerialiser.h"

Fracture::MaterialLoader::MaterialLoader()
{
}

std::shared_ptr<Fracture::Material> Fracture::MaterialLoader::LoadMaterial(const std::string& path)
{
    MaterialSerialiser loader(ISerialiser::IOMode::Open, ISerialiser::SerialiseFormat::Json);

    loader.Open(path);

     auto material = loader.ReadMaterial();

     if (material)
         return material;

    return nullptr;
}
