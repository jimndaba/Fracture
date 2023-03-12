#include "FracturePCH.h"
#include "Color.h"

Fracture::Colour Fracture::Colour::Red = Fracture::Colour(1, 0, 0, 1);
Fracture::Colour Fracture::Colour::Blue = Fracture::Colour(0, 0, 1, 1);
Fracture::Colour Fracture::Colour::Green = Fracture::Colour(0, 1, 0, 1);
Fracture::Colour Fracture::Colour::Black = Fracture::Colour(0, 0, 0, 1);
Fracture::Colour Fracture::Colour::White = Fracture::Colour(1, 1, 1, 1);
Fracture::Colour Fracture::Colour::CornflourBlue = Fracture::Colour(0.4f, 0.6f, 0.9f, 1.0f);

Fracture::Colour::Colour()
{
}

Fracture::Colour::Colour(float r, float g, float b, float a) :
	R(r), G(g), B(b), A(a)
{
}

Fracture::Colour::Colour(glm::vec4 color) :
	R(color.r), G(color.g), B(color.b), A(color.a)
{
}
