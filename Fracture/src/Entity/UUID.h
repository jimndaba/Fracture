#pragma once
#include <cstdint>
#ifndef UUID_H
#define UUID_H

namespace Fracture
{

	class UUID
	{

	public:
		UUID();
		UUID(uint32_t uuid);
		UUID(const UUID& other);

		operator uint32_t () { return m_UUID; }
		operator const uint32_t() const { return m_UUID; }

		
		UUID operator = (const UUID& other) { return *this; }
		UUID operator = (const UUID& other) const { return *this; }

		bool operator == (UUID);

	private:
		uint32_t m_UUID;

	};


}

#endif