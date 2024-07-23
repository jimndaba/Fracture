#pragma once

#ifndef AABB_H
#define AABB_H

namespace Fracture
{
	class Ray;
	struct RayHit;
	struct MeshTriangle;
	struct Vertex;

	struct Point
	{
		Point(const glm::vec3& v) :
			x(v.x), y(v.y), z(v.z)
		{

		}
		float x, y, z;
	};

	struct AABB
	{
		AABB();
		AABB(const glm::vec3& min, const glm::vec3& max);
		AABB(const Point& center, const glm::vec3& extents);
		glm::vec3 min;
		glm::vec3 max;
		glm::vec3 center;
		glm::vec3 extents;

		float Width() const;
		float Height() const;
		float Depth() const;
		bool IsValid()const;

		void Fix();

		glm::vec3 Center() const;
		glm::vec3 GetDiagonal() const;
		glm::vec3 Extents() const;

		int LongestAxis() const;

		bool RayHitAABB(const Ray& r, float& t) const;

		bool Intersects(const MeshTriangle& triangle) const;
		bool Intersects(const AABB& aabb) const;

		AABB UpdatedAABB(const glm::mat4& transform) const;
		AABB GrowAABB(const std::vector<Vertex>& v, const std::vector<unsigned int>& indxs, int start, int end);
		void Grow(const AABB& other);

		AABB Merge(const AABB& other) const;
	};


}

#endif