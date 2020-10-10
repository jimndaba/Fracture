#pragma once
#ifndef IDMANAGER_H
#define IDMANAGER_H

#include <cstdint>

namespace Fracture
{

	class IDManager
	{

	public:
		IDManager();
		~IDManager();

		static uint32_t GetID()
		{
			uint32_t id = m_currentID;
			nextID();
			return id;
		};

	private:
		static uint32_t m_currentID;
		static void nextID()
		{
			m_currentID++;
		};
	};
}

#endif
