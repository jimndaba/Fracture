#pragma once
#ifndef MATERIALSERIALISER_H
#define MATERIALSERIALISER_H


#include "Serialisation/Serialiser.h"

namespace Fracture
{
	struct Material;
	struct Uniform;

	struct MaterialSerialiser: public ISerialiser
	{
		MaterialSerialiser(ISerialiser::IOMode mode, ISerialiser::SerialiseFormat format);
		void WriteMaterial(Material* material);
		std::shared_ptr<Material> ReadMaterial();


		void writeUniforms(Fracture::Uniform uniform);
	};






}

#endif