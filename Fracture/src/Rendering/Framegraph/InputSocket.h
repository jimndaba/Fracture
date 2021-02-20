#pragma once
#ifndef INPUTSOCKET_H
#define INPUTSOCKET_H

#include <string>
#include <memory>

namespace Fracture
{

	class InputSocket
	{

	public:
		InputSocket(const std::string& Name);
		
		std::string GetName();

		static std::shared_ptr<InputSocket> Create(const std::string& Name)
		{
			return std::make_shared<InputSocket>(Name);
		}

	private:

		std::string m_name;


	};


}

#endif