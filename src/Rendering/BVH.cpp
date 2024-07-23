#include "FracturePCH.h"
#include "BVH.h"
#include "Physics/Ray.h"


glm::vec3 calculateCentroid(const glm::vec3& v1, const glm::vec3& v2) {
	return (v1 + v2) * 0.5f;
}


void Fracture::BVHTree::build(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	root = nullptr;
	splitNode(root, vertices, indices, 0, indices.size(), 0);
}

void Fracture::BVHTree::update(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
	if (root) {
		refit(vertices, indices, root.get());
	}
}

void Fracture::BVHTree::refit(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, BVHNode* node)
{
	if (node->IsLeaf()) {
		node->Bounds.GrowAABB(vertices, indices, node->start, node->count);
	}
	else {
		if (node->Left) refit(vertices, indices, node->Left.get());
		if (node->Right) refit(vertices, indices, node->Right.get());

		node->Bounds = AABB();
		if (node->Left) node->Bounds.Grow(node->Left->Bounds);
		if (node->Right) node->Bounds.Grow(node->Right->Bounds);
	}
}

Fracture::BVHRayResult Fracture::BVHTree::RayBVHTest(BVHNode* node, Ray& ray)
{
	BVHRayResult result;

	if (!node)
		return result;

	if (node->IsLeaf())
	{
		float t = 0;
		if (node->Bounds.RayHitAABB(ray,t))
		{
			result.count = node->count;
			result.start = node->start;
			result.rayHit = true;
		}
	}
	else
	{
		BVHRayResult leftResult = RayBVHTest(node->Left.get(), ray);
		BVHRayResult rightResult = RayBVHTest(node->Right.get(), ray);

		// Combine the results from left and right children
		if (leftResult.rayHit && rightResult.rayHit) {
			// If both children are hit, decide which result to return based on your logic.
			// Here, we're just returning the left result for simplicity.
			result = leftResult;
		}
		else if (leftResult.rayHit) {
			result = leftResult;
		}
		else if (rightResult.rayHit) {
			result = rightResult;
		}
	}
	return result;
}

void Fracture::BVHTree::splitNode(std::unique_ptr<BVHNode>& node, const std::vector<Vertex>& vertices, std::vector<unsigned int> indicies, int start, int count, int currentDepth)
{
	if (currentDepth > maxDepth)return;

	if (count <= 1) return; // Base case for recursion

	node = std::make_unique<BVHNode>();
	node->Depth = currentDepth;
	node->start = start;
	node->count = count;
	node->Bounds = node->Bounds.GrowAABB(vertices, indicies, start, count);
	node->Bounds.Fix();

	int axis = node->Bounds.LongestAxis();
	float center = node->Bounds.Center()[axis];


	//int mid = std::partition(indicies.begin() + start, indicies.begin() + start + count, [&](unsigned int index) {
	//	return vertices[index].Position[axis] < center;
	//	}) - indicies.begin();
	
	int mid = start;
	for (int i = start; i < start + count; i += 2) { // Step by 2 to handle triangle strip pairs
		glm::vec3 centroid = calculateCentroid(
			vertices[indicies[i]].Position,
			vertices[indicies[i + 1]].Position
		);
		if (centroid[axis] < center) {
			std::swap(indicies[i], indicies[mid]);
			std::swap(indicies[i + 1], indicies[mid + 1]);
			mid += 2;
		}
	}

	int leftCount = mid - start;
	int rightCount = count - leftCount;

	if (leftCount == 0 || rightCount == 0) return; // Base case if one side is empty


	splitNode(node->Left, vertices, indicies, start, leftCount, currentDepth + 1);
	splitNode(node->Right, vertices, indicies, mid, rightCount, currentDepth + 1);
}
