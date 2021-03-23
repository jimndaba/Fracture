#include "UIWindow.h"

Fracture::UIWindow::UIWindow(const std::string name):
	UINode(name),
	Dimensions(UIDimension())
{
}

void Fracture::UIWindow::execute(Renderer& renderer)
{
}
