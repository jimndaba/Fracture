#include "Billboard.h"
#include "GLAD/glad.h"

Fracture::Billboard::Billboard()
{
    Vertex v1;
    v1.Position = glm::vec3(0.5f, 0.5f, 0.0f );
    v1.Uvs = glm::vec2(0.0f, 0.0f);
    Vertex v2;
    v2.Position = glm::vec3(0.5f, -0.5f, 0.0f);
    v2.Uvs = glm::vec2(1.0f, 0.0f);
    Vertex v3;
    v3.Position = glm::vec3(-0.5f, -0.5f, 0.0f);
    v3.Uvs = glm::vec2(1.0f, 1.0f);
    Vertex v4;
    v4.Position = glm::vec3(-0.5f, 0.5f, 0.0f);
    v4.Uvs = glm::vec2(0.0f, 1.0f);

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    m_vertices.push_back(v1);
    m_vertices.push_back(v2);
    m_vertices.push_back(v3);
    m_vertices.push_back(v4);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glNamedBufferStorage(vbo, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_MAP_WRITE_BIT);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glNamedBufferStorage(ibo, sizeof(indices), indices, GL_MAP_WRITE_BIT);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    //uvs attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uvs));
    glEnableVertexAttribArray(1);
    m_indiceCount = sizeof(indices);
    glBindVertexArray(0);
}

Fracture::Billboard::~Billboard()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

unsigned int Fracture::Billboard::VAO()
{
    return vao;
}

int Fracture::Billboard::Indicies()
{
    return 6;
}
