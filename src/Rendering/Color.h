#pragma once
#ifndef COLOR_H
#define COLOR_H

namespace Fracture
{

	struct Colour
	{
		Colour();
		Colour(float r, float g, float b, float a);
		Colour(glm::vec4 color);

		float R, G, B, A;


		operator glm::vec3() {
			return glm::vec3(R, G, B);
		}

		operator glm::vec4() {
			return glm::vec4(R, G, B, A);
		}

		inline Colour operator = (const Colour& other)
		{
			R = other.R;
			G = other.G;
			B = other.B;
			A = other.A;
			return *this;
		}

		inline Colour operator = (const glm::vec3& other)
		{
			R = other.r;
			G = other.g;
			B = other.b;
			A = 0;
			return *this;
		}

		inline Colour operator = (const glm::vec4& other)
		{
			R = other.r;
			G = other.g;
			B = other.b;
			A = other.a;
			return *this;
		}

		inline bool operator == (const Colour& other) const {
			return R == other.R && G == other.G && B == other.B && A == other.A;
		}

		static Colour Red;
		static Colour Blue;
		static Colour Green;
		static Colour White;
		static Colour Black;
		static Colour CornflourBlue;

	};

}

#endif