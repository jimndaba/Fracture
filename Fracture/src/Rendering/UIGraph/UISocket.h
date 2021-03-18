#pragma once
#ifndef UISOCKET_H
#define UISOCKET_H

#include <string>

namespace Fracture
{

	class UISocket
	{
	public:
		UISocket(const std::string& Name);

		std::string GetName() const;

		static UISocket Make(const std::string& name)
		{
			return UISocket(name);
		}
	private:
		std::string m_name;
		
	};


}

#endif