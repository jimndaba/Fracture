#include "Renderer.h"
#include "RenderCommand.h"
#include "RenderBucket.h"
#include "Model.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "Shader.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Component/ComponentManager.h"
#include "Entity/Entity.h"
#include "Scene/Scene.h"
#include "AssetManager/AssetManager.h"
#include "Camera.h"

#ifndef GLERROR_H
#define GLERROR_H

void _check_gl_error(const char* file, int line);

///
/// Usage
/// [... some opengl calls]
/// glCheckError();
///
#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

#endif // GLERROR_H

unsigned int vao, vbo, ibo;
std::vector<Fracture::Vertex> _vertices;
std::shared_ptr<Fracture::VertexArray> _vao;
std::shared_ptr<Fracture::VertexBuffer> _vbo;

Fracture::Renderer::Renderer()
{
    m_opaqueBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
    m_camera = std::shared_ptr<Camera>(new Camera());

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    Vertex v1;
    v1.position = glm::vec3(-0.5f, -0.5f, 0.0f);
    v1.normal = glm::vec3(0);
    v1.uvs = glm::vec2(0);
    v1.tangent = glm::vec3(0);
    v1.bitangent = glm::vec3(0);
    Vertex v2;
    v2.position = glm::vec3(0.5f, -0.5f, 0.0f);
    v2.normal = glm::vec3(0);
    v2.uvs = glm::vec2(0);
    v2.tangent = glm::vec3(0);
    v2.bitangent = glm::vec3(0);
    Vertex v3;
    v3.position = glm::vec3(0.0f, 0.5f, 0.0f);
    v3.normal = glm::vec3(0);
    v3.uvs = glm::vec2(0);
    v3.tangent = glm::vec3(0);
    v3.bitangent = glm::vec3(0);

    _vertices.push_back(v1);
    _vertices.push_back(v2);
    _vertices.push_back(v3);
   

    _vao = VertexArray::create();
    _vao->bind();
    _vbo = VertexBuffer::create(_vertices, _vertices.size() * sizeof(Vertex));
    BufferElement _position = { ShaderDataType::Float3, "a_position"};
    _vbo->SetLayout({ new BufferLayout({_position}) });
    _vao->addVertexBuffer(_vbo);
    //_vao->unbind();
    /*

     
    glGenVertexArrays(1, &vao);
  
    //glBindVertexArray(vao);
   
   
    GLsizeiptr size = _vertices.size() * sizeof(Vertex);
    glBufferData(GL_ARRAY_BUFFER,size, &_vertices[0],GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
       */
   
}

Fracture::Renderer::~Renderer()
{
}

void Fracture::Renderer::update()
{
}


void Fracture::Renderer::BeginFrame(std::shared_ptr<Scene> scene)
{
    clear();
	//set state defaults
	//setViewport();
    glEnable(GL_DEPTH_TEST);	
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	m_opaqueBucket->clear();

	//collect Scene Data
    RenderScene(scene);
}

void Fracture::Renderer::RenderPasses()
{
    // for each render pass ->DrawScene();
}

void Fracture::Renderer::EndFrame()
{
	//anything to do before flush
	flush();
}

void Fracture::Renderer::flush()
{
    AssetManager::getShader("default")->use();
    glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
    AssetManager::getShader("default")->setMat4("projection", projection);
    AssetManager::getShader("default")->setMat4("view", m_camera->getViewMatrix());
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
    AssetManager::getShader("default")->setMat4("model", trans);

    _vao->bind(); 
    glDrawArrays(GL_TRIANGLES, 0, 3);   
    _vao->unbind();

    //glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //glBindVertexArray(0);

    //AssetManager::getShader("default")->unbind();
    /*
    for (auto command : m_opaqueBucket->getCommands())
    {
        std::shared_ptr<Material> material = command.material;
        std::shared_ptr<Mesh> mesh = command.mesh;
        std::shared_ptr<TransformComponent> transform = command.transform;

        material->getShader()->use();
        glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), 1280.0f / 720.0f, 0.1f, 100.0f);
        material->getShader()->setMat4("projection", projection);
        material->getShader()->setMat4("view", m_camera->getViewMatrix());
        material->getShader()->setMat4("model",transform->GetLocalTranform());

        mesh->vao->bind();
        glDrawElements(GL_TRIANGLES, mesh->ibo->GetCount(), GL_UNSIGNED_INT, 0);
        mesh->vao->unbind();
        material->getShader()->unbind();

    }
    */
}

void Fracture::Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Fracture::Renderer::clearColor(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
}

void Fracture::Renderer::setViewport(int width, int height)
{
	glViewport(0, 0, width, height);
}

void Fracture::Renderer::PushCommand(RenderCommand command)
{
	m_opaqueBucket->pushCommand(command);
}

void Fracture::Renderer::RenderEntity(std::shared_ptr<Entity> entity)
{
   
    if (!entity)
    {
        return;
    }
 
    //std::shared_ptr<RenderComponent> render = ComponentManager::GetComponent<RenderComponent>(entity->Id,ComponentType::Mesh);
    std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(entity->Id);
    /*
    if (render)
    {
        for (auto mesh : render->model->GetMeshes())
        {
            RenderCommand command;
            command.material = render->material;
            command.mesh = mesh;
            command.transform = transform;
            PushCommand(command);
        }   

        for (int i = 0; i < entity->Children().size(); i++)
        {
            RenderEntity(entity->Children()[i]);
        }
    }
    else
    {
        for (int i = 0; i < entity->Children().size(); i++)
        {
            RenderEntity(entity->Children()[i]);
        }
    }
    */
}

void Fracture::Renderer::RenderScene(std::shared_ptr<Scene> scene)
{
    //RenderEntity(scene->Root());
}

