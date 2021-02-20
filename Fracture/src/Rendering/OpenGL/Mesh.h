#pragma once
#ifndef MESH_H
#define MESH_H

#include <cstdint>

namespace Fracture
{

	class Mesh
	{
	public:
		virtual ~Mesh() {};

		virtual uint32_t RenderID() const = 0;

	};

}

#endif