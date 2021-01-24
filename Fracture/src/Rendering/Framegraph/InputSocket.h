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
		InputSocket(std::string Name);
		std::string GetName();
	private:
		//std::shared_ptr<Resource> number;
		std::string m_name;


	};


}

#endif