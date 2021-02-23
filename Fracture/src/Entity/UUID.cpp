#include "UUID.h"

#include <random>

static std::random_device s_RandomDevice;
static std::mt19937 eng(s_RandomDevice());
static std::uniform_int_distribution<uint32_t> s_UniformDistribution;



Fracture::UUID::UUID():
	m_UUID(s_UniformDistribution(eng))
{
}

Fracture::UUID::UUID(uint32_t uuid):
	m_UUID(uuid)
{
}

Fracture::UUID::UUID(const UUID& other):
	m_UUID(other.m_UUID)
{
}

bool Fracture::UUID::operator==(UUID other)
{
	return m_UUID == other.m_UUID;
}
