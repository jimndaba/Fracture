#include "UISocket.h"

Fracture::UISocket::UISocket(const std::string& Name): m_name(Name)
{
}

std::string Fracture::UISocket::GetName() const
{
    return m_name;
}
