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
#include "Rendering/Renderer2D/Renderer2D.h"
#include "Rendering/DrawCommand.h"
#include "Component/ICamera.h"

#include "Rendering/OpenGL/VertexArray.h"
#include "Rendering/OpenGL/VertexBuffer.h"
#include "Rendering/OpenGL/IndexBuffer.h"


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
	
    m_Size = glm::vec3(4, 5, 0);
    m_Position->Value = glm::vec3(10.0f, 20.0f,0.0f);
    m_Rotation->Value = glm::vec3(0.0f);
    m_Scale->Value = glm::vec3(1.0f);

	//Link Resource to sockets
	AddInputResource(PositionSocket, m_Position);
    AddInputResource(ScaleSocket, m_Scale);
    AddInputResource(RotationSocket, m_Rotation);

    m_execute = UIFunction<UIBoxNode>::Make(this,"Execute",&Fracture::UIBoxNode::Submit);
  
    AddOutputSocket(Exectue);
    AddOutputResource(Exectue,m_execute);

    {
        Vertex v1;
        v1.Position = glm::vec3(0.0f, 1.0f, 0.0f);
        v1.Normal = glm::vec3(0, 1, 0);
        v1.Uvs = glm::vec2(0.0f, 1.0f);

        Vertex v2;
        v2.Position = glm::vec3(1.0f, 0.0f, 0.0f);
        v2.Normal = glm::vec3(0, 1, 0);
        v2.Uvs = glm::vec2(1.0f, 0.0f);

        Vertex v3;
        v3.Position = glm::vec3(0.0f, 0.0f, 0.0f);
        v3.Normal = glm::vec3(0, 1, 0);
        v3.Uvs = glm::vec2(0.0f, 0.0f);

        Vertex v4;
        v4.Position = glm::vec3(0.0f, 1.0f, 0.0f);
        v4.Normal = glm::vec3(0, 1, 0);
        v4.Uvs = glm::vec2(0.0f, 1.0f);

        Vertex v5;
        v5.Position = glm::vec3(1.0f, 1.0f, 0.0f);
        v5.Normal = glm::vec3(0, 1, 0);
        v5.Uvs = glm::vec2(1.0f, 1.0f);

        Vertex v6;
        v6.Position = glm::vec3(1.0f, 0.0f, 0.0f);
        v6.Normal = glm::vec3(0, 1, 0);
        v6.Uvs = glm::vec2(1.0f, 0.0f);

        m_vertices.push_back(v1);
        m_vertices.push_back(v2);
        m_vertices.push_back(v3);
        m_vertices.push_back(v4);
        m_vertices.push_back(v5);
        m_vertices.push_back(v6);
    }

    m_VerterArray = VertexArray::Create();
    create();

   
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
    m_shader->setMat4("model", m_LocalTransform);
    m_VerterArray->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_shader->unbind();
}

void Fracture::UIBoxNode::UpdateTransforms()
{
    std::shared_ptr<UIVec3> position = std::dynamic_pointer_cast<UIVec3>(resources["Position"]);
    //glm::quat rotQuat = glm::quat(m_Rotation->Value);
    //glm::mat4 m_rotation, m_translation, m_scale;
    m_LocalTransform = glm::mat4(1.0f);
    m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(position->Value));
    //FRACTURE_INFO("Position:{} {} {} ", position->Value.x, position->Value.y, position->Value.z);
    //m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(0.5f * m_Size.x, 0.5f * m_Size.y, 0.0f));
    //
    //m_LocalTransform = glm::rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)); ///Float rotation
    //m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(-0.5f * m_Size.x, -0.5f * m_Size.y, 0.0f));
    m_LocalTransform = glm::scale(m_LocalTransform, glm::vec3(m_Size));

    //m_scale = glm::scale(m_Scale->Value);
    //m_rotation = glm::toMat4(rotQuat);
    //m_LocalTransform = m_translation * m_rotation * m_scale;
}

void Fracture::UIBoxNode::create()
{


    m_VerterArray->bind();

    m_VertexBuffer = VertexBuffer::Create(&m_vertices[0], m_vertices.size() * sizeof(Vertex));
    m_IndexBuffer = IndexBuffer::Create(&m_Indices[0], m_Indices.size() * sizeof(unsigned int));

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uvs));

    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    // vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    m_VerterArray->unbind();
}
