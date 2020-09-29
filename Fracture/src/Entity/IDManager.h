#pragma once
#ifndef IDMANAGER_H
#define IDMANAGER_H

namespace Fracture
{

	class IDManager
	{

	public:
		IDManager();
		~IDManager();

		static int GetID()
		{
			int id = m_currentID;
			nextID();
			return id;
		};

	private:
		static int m_currentID;
		static void nextID()
		{
			m_currentID++;
		};
	};
}

#endif
