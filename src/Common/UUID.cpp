#include "FracturePCH.h"
#include "UUID.h"

static std::random_device s_RandomDevice;
static std::mt19937 eng(s_RandomDevice());
static std::uniform_int_distribution<uint32_t> s_UniformDistribution;

Fracture::UUID::UUID(bool valid) :
	mValid(valid),
	m_UUID(s_UniformDistribution(eng))
{
}

Fracture::UUID::UUID(uint32_t uuid, bool valid) :
	m_UUID(uuid),
	mValid(valid)
{
}

Fracture::UUID::UUID(const UUID& other, bool valid) :
	m_UUID(other.m_UUID),
	mValid(valid)
{
}
