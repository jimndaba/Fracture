#include "FracturePCH.h"
#include "BezierSpline.h"

void Fracture::CubicBezierSpline::AddKnot(const glm::vec3& point)
{   
   
        
       
}

glm::vec3 Fracture::CubicBezierSpline::Evaluate(float t) const
{
    if (Knots.size() < 4)
    {
        throw std::runtime_error("Need at least 4 knots to evaluate the spline");
    }

    int segmentCount = (Knots.size() - 1) / 3;
    int segmentIndex = std::min(int(t * segmentCount), segmentCount - 1);
    float localT = (t * segmentCount) - segmentIndex;

    int i = segmentIndex * 3;
    glm::vec3 p0 = Knots[i].Point;
    glm::vec3 p1 = Knots[i + 1].Point;
    glm::vec3 p2 = Knots[i + 2].Point;
    glm::vec3 p3 = Knots[i + 3].Point;
    return CubicBezier(p0, p1, p2, p3, localT);
}

glm::vec3 Fracture::CubicBezierSpline::CubicBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) const
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    glm::vec3 p = uuu * p0;
    p += 3 * uu * t * p1;
    p += 3 * u * tt * p2;
    p += ttt * p3;

    return p;
}

glm::vec3  Fracture::CubicBezierSpline::CatmullRomSpline(float t) {
    int p0, p1, p2, p3;
    float fractionalPart = t * (Knots.size() - 1);
    p1 = static_cast<int>(fractionalPart);
    p0 = (p1 == 0) ? p1 : p1 - 1;
    p2 = (p1 == Knots.size() - 1) ? p1 : p1 + 1;
    p3 = (p2 == Knots.size() - 1) ? p2 : p2 + 1;

    fractionalPart -= p1;

    glm::vec3 point = 0.5f * ((2.0f * Knots[p1].Point) +
        (-Knots[p0].Point + Knots[p2].Point) * fractionalPart +
        (2.0f * Knots[p0].Point - 5.0f * Knots[p1].Point + 4.0f * Knots[p2].Point - Knots[p3].Point) * fractionalPart * fractionalPart +
        (-Knots[p0].Point + 3.0f * Knots[p1].Point - 3.0f * Knots[p2].Point + Knots[p3].Point) * fractionalPart * fractionalPart * fractionalPart);

    return point;
}

void Fracture::CubicBezierSpline::AddToEnd(const glm::vec3& point)
{
    if (!Knots.empty())
    {
        for (int i = 1; i < 4; i++)
        {
            int lastKnotIndex = Knots.size();
            glm::vec3 p2 = Knots[lastKnotIndex - 2].Point;
            glm::vec3 p3 = Knots[lastKnotIndex - 1].Point;
            glm::vec3 KnotDirection = glm::normalize(p3 - p2);
            float distance = glm::length(p3 - p2);
            glm::vec3 p = p3 + KnotDirection * distance;

            auto knot = SplineKnot{};
            auto id = UUID();
            knot.knotID = id;
            knot.Point = p;
            knot.KnotIndex = i;
            knot.TangetPointType = SplineKnot::Aligned;
            Knots.push_back(knot);           
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            int lastKnotIndex = Knots.size();
            glm::vec3 KnotDirection = glm::vec3(0, 0, 1);
            float distance = 2.0f * i;
            glm::vec3 p = point + KnotDirection * distance; 

            auto knot = SplineKnot{};
            auto id = UUID();
            knot.knotID = id;
            knot.Point = p;
            knot.KnotIndex = i;
            knot.TangetPointType = SplineKnot::Aligned;
            Knots.push_back(knot);          
        }
    }
}

void Fracture::CubicBezierSpline::RemoveLast()
{
    if (!Knots.empty())
    {
        for (int i = 0; i < 3; i++)
        {
            Knots.pop_back();           
        }
    }
}

int Fracture::CubicBezierSpline::GetKnotIndex(Fracture::UUID knotID)
{  
    for (int i = 0; i < Knots.size(); i++)
    {
        if (Knots[i].knotID == knotID)
            return i;
    }
    return -1;
}

bool Fracture::CubicBezierSpline::IsKnot(Fracture::UUID knotID)
{
    for (int i = 0; i < Knots.size(); i++)
    {
        if (Knots[i].knotID == knotID)
            return true;
    }
    return false;
}
