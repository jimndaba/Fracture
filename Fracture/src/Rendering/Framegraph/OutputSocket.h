#pragma once
#ifndef OUTPUTSOCKET_H
#define OUTPUTSOCKET_H

#include <string>
#include <memory>

namespace Fracture
{

	class OutputSocket
	{
	public:
		OutputSocket(std::string Name);

		std::string GetName();
	private:
		std::string m_name;

	};

}

#endif