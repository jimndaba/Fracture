#pragma once
#ifndef BVH_H
#define BVH_H

#include "Vertex.h"
#include "AABB.h"

namespace Fracture
{

	class Ray;

	struct BVHNode
	{
		int Depth = -1;
		int start, count;
		Fracture::AABB Bounds;
		std::unique_ptr<BVHNode> Left;
		std::unique_ptr<BVHNode> Right;
		bool IsLeaf() { return !Left && !Right; }
	};

	struct BVHRayResult
	{
		int start, count;
		bool rayHit = false;
	};

	class BVHTree
	{
	public:
		void build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void update(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
		void refit(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, BVHNode* node);
		std::unique_ptr<BVHNode> root;

		BVHRayResult RayBVHTest(BVHNode* node, Ray& ray);

	private:
		void splitNode(std::unique_ptr<BVHNode>& node, const std::vector<Vertex>& vertices, std::vector<unsigned int> indices, int start, int count, int currentDepth);	
		int maxDepth = 4;
	};
};


#endif