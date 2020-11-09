#include "Billboard.h"
#include "GLAD/glad.h"

Fracture::Billboard::Billboard()
{
    Vertex v1;
    v1.position = glm::vec3(-0.5f, -0.5f, 0.0f );
    v1.uvs = glm::vec2(1.0f, 1.0f);
    Vertex v2;
    v2.position = glm::vec3(0.5f, -0.5f, 0.0f);
    v2.uvs = glm::vec2(1.0f, 0.0f);
    Vertex v3;
    v3.position = glm::vec3(-0.5f, 0.5f, 0.0f);
    v3.uvs = glm::vec2(0.0f, 0.0f);
    Vertex v4;
    v4.position = glm::vec3(0.5f, 0.5f, 0.0f);
    v4.uvs = glm::vec2(0.0f, 1.0f);

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
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //uvs attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)offsetof(Vertex, uvs));
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
