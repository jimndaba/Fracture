#include "BufferElement.h"

Fracture::BufferElement::BufferElement()
{
}

Fracture::BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized):Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
{
}
