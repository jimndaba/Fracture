#pragma once
#ifndef MESH_H
#define MESH_H

#include <cstdint>
#include <vector>
#include <memory>

namespace Fracture
{
	struct BoundingBox
	{
		glm::vec3 min;
		glm::vec3 max;

		float Width() const
		{
			return max.x - min.x;
		}

		float Height() const
		{
			return max.y - min.y;
		}

		float Depth() const
		{
			return max.z - min.z;
		}
	};


	class Mesh
	{
	public:
		virtual ~Mesh() {};

		virtual std::string Name() {};
		virtual uint32_t RenderID() const = 0;
		virtual uint32_t CountOfIndices() {};

	};

}

#endif