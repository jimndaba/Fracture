#include "UIMultiSocket.h"

Fracture::UIMultiSocket::UIMultiSocket(const std::string& Name)
{
}

std::string Fracture::UIMultiSocket::GetName() const
{
    return m_name;
}

void Fracture::UIMultiSocket::AddChildSocket(const std::string& Name)
{
    UISocket socket = UISocket(Name);
    ChildSockets.push_back(socket);
}

Fracture::UISocket Fracture::UIMultiSocket::GetSocket(const std::string& name)
{
    for (auto socket : ChildSockets)
    {
        if (socket.GetName() == name)
            return socket;
    }
}
