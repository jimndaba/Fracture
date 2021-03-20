#include "UISectorNode.h"
#include "Rendering/OpenGL/OpenGLBase.h"

#include "Rendering/OpenGL/VertexArray.h"
#include "Rendering/OpenGL/VertexBuffer.h"
#include "Rendering/OpenGL/IndexBuffer.h"
#include "../UIResource/UIVectors.h"
#include "../UIResource/UIFunction.h"
#include "../UIResource/UIResource.h"
#include "../UIResource/UINumbers.h"
#include "Logging/Logger.h"
#include <glm\gtx\quaternion.hpp>
#include "Rendering/Shader.h"
#include "AssetManager/AssetManager.h"
#include "Rendering/Renderer.h"
#include "Rendering/Renderer2D/Renderer2D.h"
#include "Entity/Camera2D.h"
#include "Rendering/DrawCommand.h"
#include "Component/ICamera.h"


Fracture::UISectorNode::UISectorNode(const std::string Name):
	UINode(Name),
    m_Position(UIVec3::Make("Position")),
    m_Scale(UIVec3::Make("Scale")),
    m_Rotation(UIVec3::Make("Rotation")),
    m_InnerCircle(UIInt::Make("InnerRadius")),
    m_OuterCircle(UIInt::Make("OutRadius")),
    m_shader(AssetManager::getShader("UIBox")),
    PositionSocket("Position"),
    ScaleSocket("Scale"),
    RotationSocket("Rotation"),
    Exectue("Exectue"),
    InnerRadiusSocket("InnerRadius"),
    OuterRadiusSocket("OuterRadius")
{
    m_Position->Value = glm::vec3(8.0f, 0.0f, 0.0f);
    m_Rotation->Value = glm::vec3(0.0f);
    m_Scale->Value = glm::vec3(1.0f);
    m_InnerCircle->Value = 2;
    m_OuterCircle->Value = 8;

	m_execute = UIFunction<UISectorNode>::Make(this, "Execute", &Fracture::UISectorNode::Submit);


    //Link Resource to sockets
    AddInputResource(PositionSocket, m_Position);
    AddInputResource(ScaleSocket, m_Scale);
    AddInputResource(RotationSocket, m_Rotation);
    AddInputResource(InnerRadiusSocket, m_InnerCircle);
    AddInputResource(OuterRadiusSocket, m_OuterCircle);

    AddOutputSocket(Exectue);
    AddOutputResource(Exectue, m_execute);
    m_Size = glm::vec3(10, 10, 0);

    m_VerterArray = VertexArray::Create();
    create();

}

void Fracture::UISectorNode::create()
{
    //for (int i = 0; i <= 90; i++)
    //{
    //    float degInRad = i * DEG2RAD;
    //    glVertex2f(x, y); // point of the inner circle of the ring
    //
    //    for (int n = 0; n <= 4; n++)
    //    {
    //        float const t = = i * DEG2RAD * (n / 4);
    //        glVertex2f(x + sin(t) * r, y + cos(t) * r);  // points of the outer circle.
    //    }
    //}
    //for (angle = start_angle; angle < end_angle; angle += small_increment)
    //{
    //    s = sin(angle); c = cos(angle);
    //    glVertex2f(x + c * r1, y + s * r1);
    //    glVertex2f(x + c * r2, y + s * r2);
    //}

    const float DEG2RAD = 3.14159 / 180;
    const float PI2 = 3.14159f * 2.0f;
    int steps = 20;
    int segments = 8;  
    int t = 0;

    int end_angle =360/segments;
    int start_angle = 0;
    int offset = 10;
    int index = 0;

    int drawSegments = 2;

   
    
    for (int part = 0; part < drawSegments; ++part)
    {
        for (int angle = 0; angle <= steps; ++angle)
        {
            Vertex v1;
            Vertex v2;

            //float a = angle + (i / (float)samples) * length;
            float mangle = (((start_angle) + (part*end_angle )) * DEG2RAD) + (angle / (float)steps) * ((end_angle) * DEG2RAD);

            float x_inner = m_InnerCircle->Value * cos(mangle);
            float y_inner = m_InnerCircle->Value * sin(mangle);

            float x_outer = m_OuterCircle->Value * cos(mangle);
            float y_outer = m_OuterCircle->Value * sin(mangle);

            v1.Position = glm::vec3(m_Position->Value.x + x_outer, m_Position->Value.y + y_outer, m_Position->Value.z);
            v1.Uvs = glm::vec2(mangle / (float)end_angle, mangle / (float)end_angle);
            v1.Normal = glm::vec3(0, 1, 0);
            v1.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);

            v2.Position = glm::vec3(m_Position->Value.x + x_inner, m_Position->Value.y + y_inner, m_Position->Value.z);
            v2.Uvs = glm::vec2(mangle / (float)end_angle, mangle / (float)end_angle);
            v2.Normal = glm::vec3(0, 1, 0);
            v2.Tangent = glm::vec3(1.0f, 0.0f, 0.0f);

            m_vertices.push_back(v1);
            m_vertices.push_back(v2);
            
        }           
    }

    // Defining triangles.
    unsigned int indexSize = steps * 2 ;
  
    m_Indices.resize(indexSize);

    
    for (int ti = 0 ,nextrow = 0; ti < indexSize; ti++)
    {
        /* outer ring */
       for (int j = 0; j < indexSize; j++)
       {
           m_Indices[ti] = ti;
           m_Indices[ti] = ti * nextrow + j;
           m_Indices[ti] = (ti + 1) * nextrow + j;
       }
    }

    //for (int i = 1; i <= drawSegments; i++)
    //{
    //    int pos = m_Indices.size() / (i * drawSegments);
    //    m_Indices[pos] = 0xFFFF;
    //}

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

void Fracture::UISectorNode::execute(Renderer& renderer)
{
    std::shared_ptr<UIVec3> position = std::dynamic_pointer_cast<UIVec3>(resources["Position"]);
    //glm::quat rotQuat = glm::quat(m_Rotation->Value);
    //glm::mat4 m_rotation, m_translation, m_scale;
    //m_translation = glm::translate(position->Value);
    //m_scale = glm::scale(m_Scale->Value);
    //m_rotation = glm::toMat4(rotQuat);
    //m_LocalTransform = m_translation * m_rotation * m_scale;

    m_LocalTransform = glm::mat4(1.0f);
    m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(position->Value));
    //m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(0.5f * m_Size.x, 0.5f * m_Size.y, 0.0f));
    //
    //m_LocalTransform = glm::rotate(0.0f, glm::vec3(0.0f, 0.0f, 1.0f)); ///Float rotation
    //m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(-0.5f * m_Size.x, -0.5f * m_Size.y, 0.0f));
    m_LocalTransform = glm::scale(m_LocalTransform, glm::vec3(m_Size));
}


void Fracture::UISectorNode::Submit(Renderer& renderer)
{
    m_shader->use();
    m_shader->setInt("Is2D", 0);
    m_shader->setMat4("projection", renderer.ActiveCamera()->getProjectionMatrix());
    m_shader->setMat4("view", renderer.ActiveCamera()->getViewMatrix());
    m_shader->setMat4("model", m_LocalTransform);
    m_VerterArray->bind();
    //glEnable(GL_PRIMITIVE_RESTART);
    //glPrimitiveRestartIndex(0xFFFF);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertices.size());
    //glDrawElements(GL_TRIANGLE_STRIP,m_Indices.size() , GL_UNSIGNED_INT, 0);
    //glDisable(GL_PRIMITIVE_RESTART);
    m_shader->unbind();
}
