#include "FrustumPlane.h"

const float EPSILON = 0.0001f;


Fracture::FrustumPlane::FrustumPlane(void)
{
    N = glm::vec3(0, 1, 0);
    d = 0;
}

Fracture::FrustumPlane::FrustumPlane(const glm::vec3& normal, const glm::vec3& p)
{
    N = normal;
    d = -glm::dot(N, p);
}

Fracture::FrustumPlane::~FrustumPlane(void)
{
}

Fracture::FrustumPlane Fracture::FrustumPlane::FromPoints(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
{
    FrustumPlane temp;
    glm::vec3 e1 = v2 - v1;
    glm::vec3 e2 = v3 - v1;
    temp.N = glm::normalize(glm::cross(e1, e2));
    temp.d = -glm::dot(temp.N, v2);
    return temp;
}

Fracture::FrustumPlane::Where Fracture::FrustumPlane::Classify(const glm::vec3& p)
{
    float res = GetDistance(p);
    if (res > EPSILON)
        return FRONT;
    else if (res < EPSILON)
        return BACK;
    else
        return COPLANAR;
}

float Fracture::FrustumPlane::GetDistance(const glm::vec3& p)
{
    return glm::dot(N, p) + d;
}
