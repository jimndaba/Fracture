#include "FracturePCH.h"
#include "Random.h"

std::mt19937 Fracture::Random::s_FloatRandomDevice;
std::uniform_int_distribution<uint32_t> Fracture::Random::s_FloatUniformDistribution;