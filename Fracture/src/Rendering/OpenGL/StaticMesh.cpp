#include "StaticMesh.h"
#include "OpenGLBase.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "BufferAttribute.h"

Fracture::StaticMesh::StaticMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures):
	m_vertices(vertices),
	m_Indices(indices),
	m_textures(textures)
{  
    m_VerterArray = VertexArray::Create();

    m_VerterArray->bind();

    m_VertexBuffer = VertexBuffer::Create(&m_vertices[0], m_vertices.size() * sizeof(Vertex));
    m_IndexBuffer = IndexBuffer::Create(&m_Indices[0], m_Indices.size() * sizeof(unsigned int));
    
    VertexBufferLayout vertexLayout;

    vertexLayout = 
    {
        { ShaderDataType::Float3, "aPos" },   
        { ShaderDataType::Float3, "aNormal" },
        { ShaderDataType::Float2, "aTexCoords" },
        { ShaderDataType::Float3, "aTangent" },
        { ShaderDataType::Float3, "aBitangent" },          
    };
    
    SetBufferLayout(vertexLayout);

    bind();

    m_VerterArray->unbind();
}

uint32_t Fracture::StaticMesh::RenderID()const
{
    return m_VerterArray->RenderID();
}

uint32_t Fracture::StaticMesh::CountOfIndices()
{
	return m_Indices.size();
}

void Fracture::StaticMesh::SetBufferLayout(const VertexBufferLayout& layout)
{
    m_VertexBuffer->SetLayout(layout);
}

void Fracture::StaticMesh::bind()
{
   
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
   

    /*
    uint32_t attribIndex = 0;
    
    auto& m_layout = m_VertexBuffer->GetLayout();
    
    for (const auto& attribute : m_layout.GetAttributes())
    {
        auto shadertype = ShaderDataTypeToOpenGLBaseType(attribute.Type());
        glEnableVertexAttribArray(attribIndex);

        if (shadertype == GL_INT)
        {
            glVertexAttribIPointer(attribIndex,
                attribute.GetCount(),
                shadertype,
                sizeof(Vertex),
                (void*)attribute.Offset());
        }
        else
        {
            glVertexAttribPointer(attribIndex,
                attribute.GetCount(),
                shadertype,
                GL_FALSE,
                sizeof(Vertex),
                (void*)attribute.Offset());
        }

        attribIndex++;
    }
     */
    
}

std::shared_ptr<Fracture::StaticMesh> Fracture::StaticMesh::Create(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture2D>> textures)
{
	return std::make_shared<StaticMesh>(vertices,indices,textures);
}
