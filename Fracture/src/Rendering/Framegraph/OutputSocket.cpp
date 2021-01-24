#include "OutputSocket.h"

Fracture::OutputSocket::OutputSocket(std::string Name):m_name(Name)
{
}

std::string Fracture::OutputSocket::GetName()
{
	return  m_name;
}
