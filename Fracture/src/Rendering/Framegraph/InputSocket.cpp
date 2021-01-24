#include "InputSocket.h"

Fracture::InputSocket::InputSocket(std::string Name):m_name(Name)
{
}

std::string Fracture::InputSocket::GetName()
{
    return m_name;
}
