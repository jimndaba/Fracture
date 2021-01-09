#include "ICamera.h"
#include "Profiling/Profiler.h"

void Fracture::ICamera::CalcFrustumPlanes()
{
	ProfilerTimer timer("CalculateFrustum");
	glm::vec3 cN = position + front * Znear;
	glm::vec3 cF = position + front * Zfar;
	aspect_ratio = float(_width) / float(_height);
	float Hnear = 2.0f * tan(fov / 2.0f) * Znear;
	float Wnear = Hnear * aspect_ratio;
	float Hfar = 2.0f * tan(fov / 2.0f) * Zfar;
	float Wfar = Hfar * aspect_ratio;
	float hHnear = Hnear / 2.0f;
	float hWnear = Wnear / 2.0f;
	float hHfar = Hfar / 2.0f;
	float hWfar = Wfar / 2.0f;


	farPts[0] = cF + up * hHfar - right * hWfar;
	farPts[1] = cF - up * hHfar - right * hWfar;
	farPts[2] = cF - up * hHfar + right * hWfar;
	farPts[3] = cF + up * hHfar + right * hWfar;

	nearPts[0] = cN + up * hHnear - right * hWnear;
	nearPts[1] = cN - up * hHnear - right * hWnear;
	nearPts[2] = cN - up * hHnear + right * hWnear;
	nearPts[3] = cN + up * hHnear + right * hWnear;

	planes[0] = FrustumPlane::FromPoints(nearPts[3], nearPts[0], farPts[0]);
	planes[1] = FrustumPlane::FromPoints(nearPts[1], nearPts[2], farPts[2]);
	planes[2] = FrustumPlane::FromPoints(nearPts[0], nearPts[1], farPts[1]);
	planes[3] = FrustumPlane::FromPoints(nearPts[2], nearPts[3], farPts[2]);
	planes[4] = FrustumPlane::FromPoints(nearPts[0], nearPts[3], nearPts[2]);
	planes[5] = FrustumPlane::FromPoints(farPts[3], farPts[0], farPts[1]);
}

void Fracture::ICamera::GetFrustumPlanes(glm::vec4 fp[6])
{
	for (int i = 0; i < 6; i++)
		fp[i] = glm::vec4(planes[i].N, planes[i].d);	
}

bool Fracture::ICamera::IsPointInFrustum(const glm::vec3& point)
{
	for (int i = 0; i < 6; i++)
	{
		if (planes[i].GetDistance(point) < 0)
			return false;
	}
	return true;
}

bool Fracture::ICamera::IsSphereInFrustum(const glm::vec3& center, const float radius)
{
	for (int i = 0; i < 6; i++)
	{
		float d = planes[i].GetDistance(center);
		if (d < -radius)
			return false;
	}
	return true;
}

bool Fracture::ICamera::IsBoxInFrustum(const glm::vec3& min, const glm::vec3& max)
{
	ProfilerTimer timer("IsBoxInFrustum");
	for (int i = 0; i < 6; i++)
	{
		glm::vec3 p = min, n = max;
		glm::vec3 N = planes[i].N;
		if (N.x >= 0) {
			p.x = max.x;
			n.x = min.x;
		}
		if (N.y >= 0) {
			p.y = max.y;
			n.y = min.y;
		}
		if (N.z >= 0) {
			p.z = max.z;
			n.z = min.z;
		}

		if (planes[i].GetDistance(p) < 0) {
			return false;
		}
	}
	return true;
}
