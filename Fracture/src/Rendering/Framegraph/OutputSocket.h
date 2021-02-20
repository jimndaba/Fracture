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
		OutputSocket(const std::string& Name);

		std::string GetName();

		static std::shared_ptr<OutputSocket> Craete(const std::string& Name)
		{
			return std::make_shared<OutputSocket>(Name);
		}
	private:
		std::string m_name;

	};

}

#endif