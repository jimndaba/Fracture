#include "Renderer.h"
#include "RenderCommand.h"
#include "RenderBucket.h"
#include "Model.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Component/DirectLightComponent.h"
#include "Component/SpotLightComponent.h"
#include "Component/PointLightComponent.h"
#include "Component/ComponentManager.h"
#include "Component/CameraControllerComponent.h"
#include "Scene/Scene.h"
#include "RenderTarget.h"
#include "Entity/Entity.h"
#include "AssetManager/AssetManager.h"
#include "Profiling/Profiler.h"

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

Fracture::Renderer::Renderer(int width, int height):m_width(width),m_Height(height)
{
    m_opaqueBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
    SceneRenderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(m_width,m_Height,GL_FLOAT,1,true));
    glClearColor(0.3f, 0.4f, 0.6f,1.0f);
}

Fracture::Renderer::~Renderer()
{
}

void Fracture::Renderer::BeginFrame(std::shared_ptr<Scene> scene)
{
    ProfilerTimer timer("Begin Frame");
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
    ProfilerTimer timer("RenderPass");
    // for each render pass ->DrawScene();
    m_opaqueBucket->sort();
   
    SceneRenderTarget->bind();
    glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Submit();   
    SceneRenderTarget->Unbind();
 

    clearColor(0.3f, 0.5f, 0.6f);
    glClear(GL_COLOR_BUFFER_BIT);   
    Submit();
}

void Fracture::Renderer::EndFrame()
{
    
}

void Fracture::Renderer::Submit()
{
    ProfilerTimer timer("Submit");
    for(auto& command : m_opaqueBucket->getCommands())
    {
        command.material->getShader()->use();
        command.material->getShader()->setMat4("projection", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getProjectionMatrix(m_width, m_Height));
        command.material->getShader()->setMat4("view", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getViewMatrix());
        command.material->getShader()->setVec3("viewPos", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->Position);

        command.material->getShader()->setMat4("model", ComponentManager::GetComponent<TransformComponent>(command.ID)->GetWorldTransform());

        auto* uniforms = command.material->GetUniforms();
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

        std::unordered_map<std::string, UniformValueSampler>* uniformsSamplers = command.material->GetSamplerUniforms();
        for (auto it = uniformsSamplers->begin(); it != uniformsSamplers->end(); ++it)
        {
            switch (it->second.Type)
            {
            case SHADER_TYPE_SAMPLER2D:
                command.material->getShader()->setTexture(it->first, it->second.texture, it->second.Unit);
                break;
                //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
                break;
            }
        }

        for (int i = 0; i < command.Textures.size(); i++)
        {
            command.material->getShader()->setTexture(command.Textures[i]->type, command.Textures[i].get(), (int)command.Textures[i]->textureType);
        }


        Draw(command);
        command.material->getShader()->unbind();
    }
}

void Fracture::Renderer::Draw(RenderCommand command)
{
    glBindVertexArray(command.VAO);
    glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Fracture::Renderer::DrawInstanced(RenderCommand command)
{
    glMultiDrawElementsIndirect(GL_TRIANGLES, //type
        GL_UNSIGNED_INT,                   //indices represented as unsigned ints
        (GLvoid*)0,                        //start with the first draw command
        100,                               //draw 100 objects
        0);
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
    /*
    for (each entity)
    {
        push opaque command
            push shadow command
            push ....command;
    }
    */
}

void Fracture::Renderer::PushCommand(std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform)
{
    m_opaqueBucket->pushCommand(mesh,material,transform);
    //push to Shadow Passes
    //push to  Depth Pass
    //push to prost processing
}


void Fracture::Renderer::RenderEntity(std::shared_ptr<Entity> entity)
{
    ProfilerTimer timer("Render Entity");
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
            
            PushCommand(mesh, render->material, transform);
        }   
    }
}

void Fracture::Renderer::RenderScene(std::shared_ptr<Scene> scene)
{
    ProfilerTimer timer("Render Scene");
    for (std::shared_ptr<Entity> entity : scene->Entities())
    {
        RenderEntity(entity);
    }
    
}



