#include "Mesh.h"
#include "OpenGL/Vertex.h"
#include "Texture.h"
#include <GLAD/glad.h>

Fracture::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures, bool IsAnimated)
    :m_StaticVertices(vertices), m_indices(indices), m_textures(textures),m_IsAnimated(IsAnimated)
{    
   //setupMesh();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glNamedBufferStorage(VBO, m_StaticVertices.size() * sizeof(Vertex), &m_StaticVertices[0], GL_MAP_WRITE_BIT);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glNamedBufferStorage(IBO, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_MAP_WRITE_BIT);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uvs));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);

    IndexCount = (unsigned int)m_indices.size();
}

Fracture::Mesh::Mesh(std::vector<AnimatedVertex> animated_vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures, bool IsAnimated)
    : m_AnimatedVertices(animated_vertices), m_indices(indices), m_textures(textures), m_IsAnimated(IsAnimated)
{
    //setupMesh();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glNamedBufferStorage(VBO, m_AnimatedVertices.size() * sizeof(AnimatedVertex), &m_AnimatedVertices[0], GL_MAP_WRITE_BIT);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glNamedBufferStorage(IBO, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_MAP_WRITE_BIT);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Uvs));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Bitangent));

    // vertex bitangent
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT,sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, IDs));

    // vertex bitangent
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Weights));

    glBindVertexArray(0);


    IndexCount = (unsigned int)m_indices.size();
}

Fracture::Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
}

void Fracture::Mesh::SetAABB(std::shared_ptr<BoundingBox> aabb)
{
    m_bounds = aabb;
}

std::shared_ptr<Fracture::BoundingBox> Fracture::Mesh::GetAABB()
{
    return  m_bounds;
}

void Fracture::Mesh::setupMesh()
{
   
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);    

    if (m_IsAnimated)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glNamedBufferStorage(VBO, m_AnimatedVertices.size() * sizeof(AnimatedVertex), &m_AnimatedVertices[0], GL_MAP_WRITE_BIT);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glNamedBufferStorage(IBO, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_MAP_WRITE_BIT);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Uvs));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Bitangent));

        // vertex bitangent
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_INT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, IDs));

        // vertex bitangent
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(AnimatedVertex), (void*)offsetof(AnimatedVertex, Weights));

        glBindVertexArray(0);
    }
    else 
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glNamedBufferStorage(VBO, m_StaticVertices.size() * sizeof(Vertex), &m_StaticVertices[0], GL_MAP_WRITE_BIT);


        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glNamedBufferStorage(IBO, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_MAP_WRITE_BIT);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uvs));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }

    IndexCount = (unsigned int)m_indices.size();
}
