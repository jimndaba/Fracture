#include "OutputSocket.h"

Fracture::OutputSocket::OutputSocket(const std::string& Name):m_name(Name)
{
}

std::string Fracture::OutputSocket::GetName()
{
	return  m_name;
}
