#include "UILabelNode.h"
#include "../UIResource/UIVectors.h"
#include "../UIResource/UIFunction.h"
#include "../UIResource/UIResource.h"
#include "../UIResource/UINumbers.h"
#include "../UIResource/UIText.h"
#include "Rendering/Renderer2D/Renderer2D.h"

Fracture::UILabelNode::UILabelNode(const std::string Name):
	UINode(Name),
	m_Position(UIVec3::Make("Position")),
	m_Scale(UIVec3::Make("Scale")),
	m_Rotation(UIVec3::Make("Rotation")),
	m_text(UIText::Make("Text")),	
	PositionSocket("Position"),
	ScaleSocket("Scale"),
	RotationSocket("Rotation"),
	TextSocket("Text"),
	Exectue("Exectue")
{

	m_Position->Value = glm::vec3(100.0f, 100.0f, 0.0f);
	m_Rotation->Value = glm::vec3(0.0f);
	m_Scale->Value = glm::vec3(1.0f);
	m_text->Value = "Empty";
	

	m_execute = UIFunction<UILabelNode>::Make2D(this, "Execute", &Fracture::UILabelNode::Submit);


	//Link Resource to sockets
	AddInputResource(PositionSocket, m_Position);
	AddInputResource(ScaleSocket, m_Scale);
	AddInputResource(RotationSocket, m_Rotation);
	AddInputResource(TextSocket, m_text);
	
	AddOutputSocket(Exectue);
	AddOutputResource(Exectue, m_execute);
}

void Fracture::UILabelNode::execute(Renderer& renderer)
{
}

void Fracture::UILabelNode::Submit(Renderer2D& renderer)
{
	renderer.DrawText2D(m_text->Value, m_Position->Value.x, m_Position->Value.y, 1.0f, glm::vec3(1.0f,1.0f,0.0f));
}

void Fracture::UILabelNode::SetText(const std::string& text)
{
	m_text->Value = text;
}
