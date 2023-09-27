#pragma once
#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Fracture
{


	class Random
	{
	public:
		static void Init()
		{
			s_FloatRandomDevice.seed(std::random_device()());
		}


		static float Float()
		{
			return (float)s_FloatUniformDistribution(s_FloatRandomDevice) / (float)std::numeric_limits<uint32_t>::max();
		}


	private:
		static std::mt19937 s_FloatRandomDevice;
		static std::uniform_int_distribution<uint32_t> s_FloatUniformDistribution;
	};




}

#endif