#pragma once
#ifndef BEZIERSPLINE_H
#define BEZIERSPLINE_H


namespace Fracture
{
	
	struct SplineKnot
	{
		enum TangetPointTypeOption
		{
			Broken,
			Aligned,
			Mirrored
		}; 
		UUID knotID;
		int KnotIndex = -1;
		glm::vec3 Point;
		TangetPointTypeOption TangetPointType;
	};

	struct CubicBezierSpline
	{
		std::vector<SplineKnot> Knots;
		void AddKnot(const glm::vec3& point);
		glm::vec3 Evaluate(float t) const;
		glm::vec3 CubicBezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t) const;
		glm::vec3 CatmullRomSpline(float t);
		void AddToEnd(const glm::vec3& point);
		void RemoveLast();

		int GetKnotIndex(Fracture::UUID knotID);
		bool IsKnot(Fracture::UUID knotID);
	};







}

#endif