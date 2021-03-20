#include "UIOutputNode.h"
#include "Rendering/Renderer.h"
#include "../UIResource/UIResource.h"
#include "../UIResource/UICollections.h"
#include "../UIResource/UIFunction.h"
#include "UIBoxNode.h"
#include "Rendering/RenderTarget.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include <typeinfo>

Fracture::UIOutputNode::UIOutputNode(Renderer2D& renderer2D,const std::string Name,int width, int height):
	UINode(Name),
    Submit("Submit"),
    Submit2D("Submit2D"),
    m_2DRenderer(renderer2D)
{
	m_GraphCommands = std::make_shared<UIVector<DrawCommand>>("UICommands");

    RenderOut = RenderTarget::CreateRenderTarget("Sink_Color_In", width, height, AttachmentTarget::Texture2D, FormatType::Float, 1, false);
    
    AddMultiInputSocket(Submit);
    AddMultiInputSocket(Submit2D);

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    {
        glGenVertexArrays(1, &quadVAO);

        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);

        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

}

void Fracture::UIOutputNode::execute(Renderer& renderer)
{  
    glDisable(GL_CULL_FACE);
    RenderOut->bind(); 
    renderer.clear();
    for (const auto& input : Submit.ChildSockets)
    {
       const auto& func = resources[input.GetName()];
       func->submit(renderer);  
    }   
    
    for (const auto& input : Submit2D.ChildSockets)
    {
        const auto& func = resources[input.GetName()];
        func->submit2D(m_2DRenderer);
    }
    RenderOut->Unbind();
    glEnable(GL_CULL_FACE);
}

void Fracture::UIOutputNode::AddSubmit(UISocket input)
{
    m_submitInputs.push_back(input);
}

std::vector<Fracture::DrawCommand> Fracture::UIOutputNode::RenderCommands()
{
	return m_GraphCommands->Value;
}
