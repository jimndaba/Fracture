#include "InputSocket.h"

Fracture::InputSocket::InputSocket(const std::string& Name):m_name(Name)
{
}

std::string Fracture::InputSocket::GetName()
{
    return m_name;
}
