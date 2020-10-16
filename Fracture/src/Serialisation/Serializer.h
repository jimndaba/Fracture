#pragma once
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace Fracture
{

	class Serializer
	{

	public:
		Serializer();
		~Serializer();

	};

}

#endif