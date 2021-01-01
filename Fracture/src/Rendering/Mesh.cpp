#include "Mesh.h"
#include "Vertex.h"
#include "Texture.h"

Fracture::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures, std::shared_ptr<Material> material)
    :m_vertices(vertices), m_indices(indices), m_textures(textures), m_material(material)
{    
    setupMesh();
}

Fracture::Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

void Fracture::Mesh::setupMesh()
{
   
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glNamedBufferStorage(VBO, m_vertices.size() * sizeof(Vertex), &m_vertices[0],GL_MAP_WRITE_BIT);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glNamedBufferStorage(IBO, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_MAP_WRITE_BIT);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uvs));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);

    


}
