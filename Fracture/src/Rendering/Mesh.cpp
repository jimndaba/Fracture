#include "Mesh.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "BufferLayout.h"
#include "BufferElement.h"
#include "Texture.h"

Fracture::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures):m_vertices(vertices),m_indices(indices),m_textures(textures)
{
    
    setupMesh();
}

Fracture::Mesh::~Mesh()
{
}

void Fracture::Mesh::setupMesh()
{
    vao = VertexArray::create(); //std::shared_ptr<VertexArray>(new VertexArray());
    vbo = VertexBuffer::create(m_vertices,(size_t)m_vertices.size() * sizeof(Vertex));    
    BufferElement position = { ShaderDataType::Float3, "a_position" };
    BufferElement normal = { ShaderDataType::Float3, "a_normal" };
    BufferElement uv = { ShaderDataType::Float2, "a_uv" };
    BufferElement tangent = { ShaderDataType::Float3, "a_tangent" };
    BufferElement bitangent = { ShaderDataType::Float3, "a_bitangent" };

    vbo->SetLayout(new BufferLayout({
            { position },
            { normal },
            { uv },
            { tangent },
            { bitangent }
        }));

    ibo = IndexBuffer::create(m_indices, (size_t)m_indices.size() * sizeof(unsigned int));
    vao->addVertexBuffer(vbo);
    vao->setIndexBuffer(ibo);

}
