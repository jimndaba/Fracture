#include "UIBoxNode.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "../UIResource/UIVectors.h"
#include "../UIResource/UIFunction.h"
#include "../UIResource/UIResource.h"
#include "Logging/Logger.h"
#include <glm\gtx\quaternion.hpp>
#include "Rendering/Shader.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/DrawCommand.h"
#include "Component/ICamera.h"


Fracture::UIBoxNode::UIBoxNode(const std::string Name):
	UINode(Name),
	PositionSocket("Position"),
    ScaleSocket("Scale"),
    RotationSocket("Rotation"),	
    Exectue("Exectue"),
	m_Position(UIVec3::Make("Position")),
    m_Scale(UIVec3::Make("Scale")),
    m_Rotation(UIVec3::Make("Rotation")),
    m_shader(AssetManager::getShader("UIBox"))
{
	AddInputSocket(PositionSocket);
    AddInputSocket(ScaleSocket);
    AddInputSocket(RotationSocket);
	
	
    m_Position->Value = glm::vec3(0.0f, 0.0f,0.0f);
    m_Rotation->Value = glm::vec3(0.0f);
    m_Scale->Value = glm::vec3(1.0f);

	//Link Resource to sockets
	AddInputResource(PositionSocket, m_Position);
    AddInputResource(ScaleSocket, m_Scale);
    AddInputResource(RotationSocket, m_Rotation);

    m_execute = UIFunction<UIBoxNode>::Make(this,"Execute",&Fracture::UIBoxNode::Submit);
  
    AddOutputSocket(Exectue);
    AddOutputResource(Exectue,m_execute);

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

    UpdateTransforms();
}

void Fracture::UIBoxNode::execute(Renderer& renderer)
{	
    UpdateTransforms();
}

void Fracture::UIBoxNode::Submit(Renderer& renderer)
{       
    m_shader->use();
    m_shader->setMat4("projection",renderer.ActiveCamera()->getProjectionMatrix());
    m_shader->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
    m_shader->setMat4("model", m_LocalTransform);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_shader->unbind();
}

void Fracture::UIBoxNode::UpdateTransforms()
{
    std::shared_ptr<UIVec3> position = std::dynamic_pointer_cast<UIVec3>(resources["Position"]);
    glm::quat rotQuat = glm::quat(m_Rotation->Value);
    glm::mat4 m_rotation, m_translation, m_scale;
    m_translation = glm::translate(position->Value);
    m_scale = glm::scale(m_Scale->Value);
    m_rotation = glm::toMat4(rotQuat);
    m_LocalTransform = m_translation * m_rotation * m_scale;
}
