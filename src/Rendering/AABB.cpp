#include "FracturePCH.h"
#include "AABB.h"
#include <Physics/Ray.h>
#include "Mesh.h"
#include "Common/Math.h"
#include "Common/Frustum.h"
#include "Vertex.h"

Fracture::AABB::AABB() :
    min(glm::vec3(0)), max(glm::vec3(0))
{
}

Fracture::AABB::AABB(const glm::vec3& Min, const glm::vec3& Max) :
    min(Min), max(Max)
{
    if (!IsValid())
    {
        Fix();
    }
}

Fracture::AABB::AABB(const Point& Cntr, const glm::vec3& ext) :
    center(glm::vec3(Cntr.x, Cntr.y, Cntr.z)), extents(ext)
{
    min = center - extents;
    max = center + extents;

    if (!IsValid())
    {
        Fix();
    }
}

float Fracture::AABB::Width() const
{
    return 0.0f;
}

float Fracture::AABB::Height() const
{
    return 0.0f;
}

float Fracture::AABB::Depth() const
{
    return 0.0f;
}

bool Fracture::AABB::IsValid() const
{
    return min.x < max.x&& min.y < max.y&& min.z < max.z;
}


void Fracture::AABB::Fix()
{
    if (min.x > max.x) {
        auto a = min.x;
        auto b = max.x;

        min.x = b;
        max.x = a;
    }
    if (min.y > max.y) {
        auto a = min.y;
        auto b = max.y;

        min.y = b;
        max.y = a;
    }
    if (min.z > max.z) {
        // Swap
        auto a = min.z;
        auto b = max.z;

        min.z = b;
        max.z = a;
    }
}

glm::vec3 Fracture::AABB::Center() const
{
    return min + Extents();;
}

glm::vec3 Fracture::AABB::GetDiagonal() const
{
    return max - min;
}

glm::vec3 Fracture::AABB::Extents() const
{
    return 0.5f * (max - min);
}

int Fracture::AABB::LongestAxis() const
{
    glm::vec3 extents = Extents();
    if (extents.x > extents.y && extents.x > extents.z) {
        return 0; // x-axis
    }
    else if (extents.y > extents.x && extents.y > extents.z) {
        return 1; // y-axis
    }
    else {
        return 2; // z-axis
    }
}

bool Fracture::AABB::RayHitAABB(const Ray& r, float& t) const
{
    glm::vec3 dirfrac;
    // r.dir is unit direction vector of ray
    dirfrac.x = 1.0f / r.direction.x;
    dirfrac.y = 1.0f / r.direction.y;
    dirfrac.z = 1.0f / r.direction.z;
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    const glm::vec3& lb = min;
    const glm::vec3& rt = max;
    float t1 = (lb.x - r.origin.x) * dirfrac.x;
    float t2 = (rt.x - r.origin.x) * dirfrac.x;
    float t3 = (lb.y - r.origin.y) * dirfrac.y;
    float t4 = (rt.y - r.origin.y) * dirfrac.y;
    float t5 = (lb.z - r.origin.z) * dirfrac.z;
    float t6 = (rt.z - r.origin.z) * dirfrac.z;

    float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
    float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (tmax < 0)
    {
        t = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        t = tmax;
        return false;
    }

    t = tmin;
    return true;
}

bool Fracture::AABB::Intersects(const MeshTriangle& triangle) const
{
    float p0, p1, p2, r;

    // Compute box center and extents of AABoundingBox (if not already given in that format)
    glm::vec3 c = (max + min) * 0.5f;
    glm::vec3 e = max - c;

    // Translate triangle as conceptually moving AABB to origin
    glm::vec3 v0 = triangle.v1 - c;
    glm::vec3 v1 = triangle.v2 - c;
    glm::vec3 v2 = triangle.v3 - c;

    // Compute edge vectors for triangle
    glm::vec3 f0 = v1 - v0;
    glm::vec3 f1 = v2 - v1;
    glm::vec3 f2 = v0 - v2;

    // Test axes a00..a22 (category 3)
    glm::vec3 a00 = glm::vec3(0, -f0.z, f0.y);
    glm::vec3 a01 = glm::vec3(0, -f1.z, f1.y);
    glm::vec3 a02 = glm::vec3(0, -f2.z, f2.y);
    glm::vec3 a10 = glm::vec3(f0.z, 0, -f0.x);
    glm::vec3 a11 = glm::vec3(f1.z, 0, -f1.x);
    glm::vec3 a12 = glm::vec3(f2.z, 0, -f2.x);
    glm::vec3 a20 = glm::vec3(-f0.y, f0.x, 0);
    glm::vec3 a21 = glm::vec3(-f1.y, f1.x, 0);
    glm::vec3 a22 = glm::vec3(-f2.y, f2.x, 0);

    // Test axis a00
    p0 = glm::dot(v0, a00);
    p1 = glm::dot(v1, a00);
    p2 = glm::dot(v2, a00);
    r = e.y * glm::abs(f0.z) + e.z * glm::abs(f0.y);

    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis

    }

    // Test axis a01
    p0 = glm::dot(v0, a01);
    p1 = glm::dot(v1, a01);
    p2 = glm::dot(v2, a01);
    r = e.y * glm::abs(f1.z) + e.z * glm::abs(f1.y);

    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis

    }

    // Test axis a02
    p0 = glm::dot(v0, a02);
    p1 = glm::dot(v1, a02);
    p2 = glm::dot(v2, a02);
    r = e.y * glm::abs(f2.z) + e.z * glm::abs(f2.y);

    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis

    }

    // Test axis a10
    p0 = glm::dot(v0, a10);
    p1 = glm::dot(v1, a10);
    p2 = glm::dot(v2, a10);
    r = e.x * glm::abs(f0.z) + e.z * glm::abs(f0.x);
    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis

    }

    // Test axis a11
    p0 = glm::dot(v0, a11);
    p1 = glm::dot(v1, a11);
    p2 = glm::dot(v2, a11);
    r = e.x * glm::abs(f1.z) + e.z * glm::abs(f1.x);

    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis

    }

    // Test axis a12
    p0 = glm::dot(v0, a12);
    p1 = glm::dot(v1, a12);
    p2 = glm::dot(v2, a12);
    r = e.x * glm::abs(f2.z) + e.z * glm::abs(f2.x);

    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis
    }

    // Test axis a20
    p0 = glm::dot(v0, a20);
    p1 = glm::dot(v1, a20);
    p2 = glm::dot(v2, a20);
    r = e.x * glm::abs(f0.y) + e.y * glm::abs(f0.x);

    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis

    }

    // Test axis a21
    p0 = glm::dot(v0, a21);
    p1 = glm::dot(v1, a21);
    p2 = glm::dot(v2, a21);
    r = e.x * glm::abs(f1.y) + e.y * glm::abs(f1.x);

    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis

    }

    // Test axis a22
    p0 = glm::dot(v0, a22);
    p1 = glm::dot(v1, a22);
    p2 = glm::dot(v2, a22);
    r = e.x * glm::abs(f2.y) + e.y * glm::abs(f2.x);

    if (glm::max(-Math::Max(p0, p1, p2), Math::Min(p0, p1, p2)) > r) {

        return false; // Axis is a separating axis

    }

    // Test the three axes corresponding to the face normals of AABB b (category 1).
    // Exit if...
    // ... [-extents.x, extents.x] and [min(v0.x,v1.x,v2.x), max(v0.x,v1.x,v2.x)] do not overlap
    if (Math::Max(v0.x, v1.x, v2.x) < -e.x || Math::Min(v0.x, v1.x, v2.x) > e.x) {

        return false;

    }
    // ... [-extents.y, extents.y] and [min(v0.y,v1.y,v2.y), max(v0.y,v1.y,v2.y)] do not overlap
    if (Math::Max(v0.y, v1.y, v2.y) < -e.y || Math::Min(v0.y, v1.y, v2.y) > e.y) {

        return false;

    }
    // ... [-extents.z, extents.z] and [min(v0.z,v1.z,v2.z), max(v0.z,v1.z,v2.z)] do not overlap
    if (Math::Max(v0.z, v1.z, v2.z) < -e.z || Math::Min(v0.z, v1.z, v2.z) > e.z) {

        return false;

    }

    // Test separating axis corresponding to triangle face normal (category 2)
    // Face Normal is -ve as Triangle is clockwise winding (and XNA uses -z for into screen)
    FrustumPlane plane;
    plane.normal = glm::normalize(glm::cross(f1, f0));
    plane.distance = glm::dot(plane.normal, triangle.v1);

    return Math::IntersectionAABBPlane(min, max, plane);
}

bool Fracture::AABB::Intersects(const AABB& b) const
{
    return (min.y <= b.max.x && max.x >= b.min.x) &&
        (min.y <= b.max.y && max.y >= b.min.y) &&
        (min.z <= b.max.z && max.z >= b.min.z);
}

Fracture::AABB Fracture::AABB::Merge(const AABB& other) const
{
    AABB result;
    result.min.x = glm::min(min.x, other.min.x);
    result.min.y = glm::min(min.y, other.min.y);
    result.min.z = glm::min(min.z, other.min.z);
    result.max.x = glm::max(max.x, other.max.x);
    result.max.y = glm::max(max.y, other.max.y);
    result.max.z = glm::max(max.z, other.max.z);
    return result;
}

Fracture::AABB Fracture::AABB::UpdatedAABB(const glm::mat4& transform) const
{
	AABB aabb;
	// compute column multiplies for the AABB min
	glm::vec3 min_c1 = glm::vec3(transform[0].x, transform[0].y, transform[0].z) * min.x;
	glm::vec3 min_c2 = glm::vec3(transform[1].x, transform[1].y, transform[1].z) * min.y;
	glm::vec3 min_c3 = glm::vec3(transform[2].x, transform[2].y, transform[2].z) * min.z + glm::vec3(transform[3].x, transform[3].y, transform[3].z);
	// place 4th column add here for free add (MAD)

	// compute column multiplies for the AABB max

	glm::vec3 max_c1 = glm::vec3(transform[0].x, transform[0].y, transform[0].z) * max.x;
	glm::vec3 max_c2 = glm::vec3(transform[1].x, transform[1].y, transform[1].z) * max.y;
	glm::vec3 max_c3 = glm::vec3(transform[2].x, transform[2].y, transform[2].z) * max.z + glm::vec3(transform[3].x, transform[3].y, transform[3].z);

	// minimize and maximize the resulting transforms
	glm::vec3 tmin = glm::min(min_c1, max_c1) + glm::min(min_c2, max_c2) + glm::min(min_c3, max_c3);
	glm::vec3 tmax = glm::max(min_c1, max_c1) + glm::max(min_c2, max_c2) + glm::max(min_c3, max_c3);

	aabb.min = tmin;
	aabb.max = tmax;


	return aabb;
}

Fracture::AABB Fracture::AABB::GrowAABB(const std::vector<Vertex>& v,const std::vector<unsigned int>& indxs, int start, int end)
{
    AABB bound; 

    // Initialize min and max with the first vertex in the range
    glm::vec3 _min = v[indxs[start]].Position;
    glm::vec3 _max = v[indxs[start]].Position;

    for (int i = start + 1; i < start + end; ++i) {
        const glm::vec3& pos = v[indxs[i]].Position;
        _min = glm::min(_min, pos);
        _max = glm::max(_max, pos);
    }

    bound.min = _min;
    bound.max = _max;
    return bound;
}

void Fracture::AABB::Grow(const AABB& other)
{
    min = glm::min(min, other.min);
    max = glm::max(max, other.max);
}