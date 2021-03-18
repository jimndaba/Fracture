#pragma once
#ifndef UIMULTISOCKET_H
#define UIMULTISOCKET_H

#include <string>
#include <vector>
#include "UISocket.h"

namespace Fracture
{
	
	class UIMultiSocket
	{
	public:
		UIMultiSocket(const std::string& Name);

		std::string GetName() const;

		static UIMultiSocket Make(const std::string& name)
		{
			return UIMultiSocket(name);
		}

		void AddChildSocket(const std::string& Name);
		UISocket GetSocket(const std::string& name);

		std::vector<UISocket> ChildSockets;

	private:
		std::string m_name;

	};


}

#endif
