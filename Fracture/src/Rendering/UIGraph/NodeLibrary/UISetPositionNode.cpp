#include "UISetPositionNode.h"
#include "Rendering/UIGraph/UIResource/UIVectors.h"
#include "Rendering/UIGraph/UIResource/UIResource.h"

Fracture::UISetPositionNode::UISetPositionNode(const std::string Name):
	UINode(Name),
	Output("Output"),
	m_Position(UIVec3::Make("Position"))
{
	m_Position->Value = glm::vec3(0.0f);
	AddOutputSocket(Output);
	AddOutputResource(Output, m_Position);
}

void Fracture::UISetPositionNode::execute(Renderer& renderer)
{
}

void Fracture::UISetPositionNode::SetPosition(const glm::vec3& position)
{
	m_Position->Value = position;
}
