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

Fracture::Renderer::Renderer(GameWindow& window):m_window(window)
{
    m_opaqueBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
    m_camera = std::shared_ptr<Camera>(new Camera());
}

Fracture::Renderer::~Renderer()
{
}

void Fracture::Renderer::update(float dt)
{
    m_camera->update(dt);
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
	Submit();
}

void Fracture::Renderer::Submit()
{
    for(const auto& command : m_opaqueBucket->getCommands())
    {
        command.material->getShader()->use();
        //set material settings
        command.material->getShader()->setMat4("projection", m_camera->getProjectionMatrix(&m_window));
        command.material->getShader()->setMat4("view", m_camera->getViewMatrix());
       

        // set uniform state of material
        std::unordered_map<std::string, UniformValue>* uniforms = command.material->GetUniforms();
        for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
        {
            switch (it->second.Type)
            {
            case SHADER_TYPE_BOOL:
                command.material->getShader()->setBool(it->first, it->second.Bool);
                break;
            case SHADER_TYPE_INT:
                command.material->getShader()->setInt(it->first, it->second.Int);
                break;
            case SHADER_TYPE_FLOAT:
                command.material->getShader()->setFloat(it->first, it->second.Float);
                break;
            case SHADER_TYPE_VEC2:
                command.material->getShader()->setVec2(it->first, it->second.Vec2);
                break;
            case SHADER_TYPE_VEC3:
                command.material->getShader()->setVec3(it->first, it->second.Vec3);
                break;
            case SHADER_TYPE_VEC4:
                command.material->getShader()->setVec4(it->first, it->second.Vec4);
                break;
            case SHADER_TYPE_MAT2:
                command.material->getShader()->setMat2(it->first, it->second.Mat2);
                break;
            case SHADER_TYPE_MAT3:
                command.material->getShader()->setMat3(it->first, it->second.Mat3);
                break;
            case SHADER_TYPE_MAT4:
                command.material->getShader()->setMat4(it->first, it->second.Mat4);
                break;
            default:
                //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
                break;
            }
        }

        std::map<std::string, UniformValueSampler>* uniformsSamplers = command.material->GetSamplerUniforms();
        for (auto it = uniformsSamplers->begin(); it != uniformsSamplers->end(); ++it)
        {
            switch (it->second.Type)
            {
            case SHADER_TYPE_SAMPLER2D:
                command.material->getShader()->setTexture(it->first, it->second.texture, it->second.Unit);
                break;
            default:
                //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
                break;
            }
        }

        command.material->getShader()->setMat4("model", command.transform->GetWorldTransform());

        glBindVertexArray(command.VAO);
        glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        command.material->getShader()->unbind();
    }    
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
 
    std::shared_ptr<RenderComponent> render = ComponentManager::GetComponent<RenderComponent>(entity->Id);
    std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(entity->Id);
  
    if (render)
    {
        for (auto mesh : render->model->GetMeshes())
        {
            RenderCommand command;
            command.material = render->material;
            command.VAO = mesh->VAO;
            command.indiceSize = (GLint)mesh->GetIndices().size();
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
  
}

void Fracture::Renderer::RenderScene(std::shared_ptr<Scene> scene)
{
    RenderEntity(scene->Root());
}

