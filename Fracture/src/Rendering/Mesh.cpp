#include "Mesh.h"
#include "Vertex.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"

Fracture::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures):m_vertices(vertices),m_indices(indices),m_textures(textures)
{
    /*
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    */

}

Fracture::Mesh::~Mesh()
{
}

void Fracture::Mesh::setupMesh()
{
    vao = std::shared_ptr<VertexArray>(new VertexArray());

    vbo = std::shared_ptr <VertexBuffer>(new VertexBuffer(m_vertices, m_vertices.size() * sizeof(Vertex)));

    ibo = std::shared_ptr<IndexBuffer>(new IndexBuffer(m_indices, m_indices.size() * sizeof(unsigned int)));


}
