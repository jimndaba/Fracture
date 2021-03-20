#include "Renderer2D.h"
#include "Rendering/OpenGL/OpenGLBase.h"
#include "Rendering/Shader.h"
#include "Rendering/Font.h"
#include "Rendering/OpenGL/VertexArray.h"
#include "Rendering/OpenGL/VertexBuffer.h"
#include "AssetManager/AssetManager.h"
#include "Entity/Camera2D.h"

Fracture::Renderer2D::Renderer2D():
    m_TextShader(AssetManager::getShader("FontShader"))
{
    m_width = 1920;
    m_height = 1080;
}

void Fracture::Renderer2D::SetFont(const std::string& font)
{
    m_CurrentFont = AssetManager::getFont(font);
}

void Fracture::Renderer2D::SetCamera(const std::shared_ptr<Camera2D>& camera)
{
    m_Camera2D =  camera;
    m_Camera2D->setProjection(m_width, m_height);
}

std::shared_ptr<Fracture::Camera2D> Fracture::Renderer2D::ActiveCamera()
{
    return m_Camera2D;
}

void Fracture::Renderer2D::DrawText2D(const std::string& text, float x, float y, float scale, glm::vec3 color)
{
    
    m_TextShader->use();
    
    m_TextShader->setColor3("textColor", color);
    m_TextShader->setInt("Is2D", 1);
    m_TextShader->setMat4("projection", m_Camera2D->getProjectionMatrix());

    
     glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // activate corresponding render state	
    //glDisable(GL_CULL_FACE);
   
    m_CurrentFont->bind();
    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_CurrentFont->GetCharacters()[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos,       1.0, 0.0 },
            { xpos ,    ypos,       0.0, 0.0 },
            

            { xpos,     ypos + h,   0.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 1.0 },
            { xpos + w, ypos    ,   1.0, 0.0 },
            
        };
        // render glyph texture over quad
        m_TextShader->setTexture("text", ch.TextureID, 0);
        
        // update content of VBO memory
        m_CurrentFont->GetBuffer()->SetData(vertices, 0, sizeof(vertices));       
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    m_CurrentFont->unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_TextShader->unbind();
    glDisable(GL_BLEND);
}

void Fracture::Renderer2D::setViewport(int width, int height)
{
    glViewport(0, 0, width, height);
    if (m_Camera2D)
    {
        m_Camera2D->setProjection(width, height);
    }
}
