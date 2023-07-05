#pragma once
#ifndef UUID_H
#define UUID_H

namespace Fracture
{
	struct UUID
	{

	public:
		UUID();
		UUID(uint32_t uuid, bool valid = true);
		UUID(const UUID& other, bool valid = true);

		operator uint32_t () { return m_UUID; }
		operator const uint32_t() const { return m_UUID; }

		//operator bool() const
		//{
		//	return true;
		//}

		inline UUID operator = (const UUID& other) { m_UUID = other.m_UUID; return *this; }
		inline UUID operator = (const UUID& other) const { m_UUID = other.m_UUID;  return *this; }
		inline bool operator == (const UUID& other) const {
			return m_UUID == other.m_UUID;
		}


		inline bool operator < (const UUID& other) const {
			return m_UUID < other.m_UUID;
		}


		bool IsValid() { return mValid; };

		uint32_t GetValue() const {
			return m_UUID;
		}

	private:
		mutable uint32_t m_UUID;
		bool mValid = false;

	};


}

namespace std
{
	template<>
	struct hash<Fracture::UUID>
	{
		std::size_t operator()(const Fracture::UUID& uuid)const
		{
			return hash<uint32_t>()((uint32_t)uuid);
		}
	};

}


#endif