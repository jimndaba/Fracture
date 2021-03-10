#pragma once
#ifndef MESH_H
#define MESH_H

#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include "glm/glm.hpp"

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

		virtual std::string Name() { return m_Name; };
		virtual uint32_t RenderID() const = 0;
		virtual uint32_t CountOfIndices() { return 0; };

		std::shared_ptr<BoundingBox> GetAABB()
		{
			return m_AABB;
		}
		void SetAABB(const std::shared_ptr<BoundingBox>& aabb)
		{
			m_AABB = aabb;
		}

		std::string GetName()
		{
			return m_Name;
		}
		void SetName(const std::string& name)
		{
			m_Name = name;
		}
		void SetMaterialIndex(uint32_t index)
		{
			m_MaterialIndex = index;
		}
		uint32_t MaterialIndex()
		{
			return m_MaterialIndex;
		}

	private:
		std::shared_ptr<BoundingBox> m_AABB;
		std::string m_Name;
		uint32_t m_MaterialIndex;

	};

}

#endif