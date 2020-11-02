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
#include "Component/LightComponent.h"
#include "Component/ILight.h"
#include "Component/ComponentManager.h"
#include "Component/CameraControllerComponent.h"
#include "Component/TagComponent.h"

#include "Game/Game.h"
#include "Scene/Scene.h"
#include "RenderTarget.h"
#include "Entity/Entity.h"
#include "AssetManager/AssetManager.h"
#include "Profiling/Profiler.h"
#include "Logging/Logger.h"
#include "Event/Eventbus.h"
#include "Physics/PhysicsManager.h"
#include "DebugLine.h"

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

std::vector<std::shared_ptr<Fracture::DebugLine>> Fracture::Renderer::m_DebugDraws;
std::vector<std::shared_ptr<Fracture::DebugLine>> Fracture::Renderer::m_DebugDrawsRetained;
bool Fracture::Renderer::m_isDebugRender;

Fracture::Renderer::Renderer(int width, int height):m_width(width),m_Height(height)
{
    m_opaqueBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
    SceneRenderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(m_width,m_Height,GL_FLOAT,1,true));
    glClearColor(0.3f, 0.4f, 0.6f,1.0f);
    glEnable(GL_DEPTH_TEST);
 
}

Fracture::Renderer::~Renderer()
{
}

void Fracture::Renderer::onInit()
{    
    FRACTURE_INFO("Renderer Init");
    Game::GetEventbus()->Subscribe(this ,& Fracture::Renderer::onWindowResize);
    m_isDebugRender = false;
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
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    Submit();        
    
    if (m_isDebugRender)
    {
        PhysicsManager::DrawDebug();  
        RenderDebug();
        RenderDebugRetained();
    }
   
    SceneRenderTarget->Unbind();
  
    //glDisable(GL_DEPTH_TEST);   
    //clearColor(0.3f, 0.5f, 0.6f);
    //glClear(GL_COLOR_BUFFER_BIT);   
    //Submit();
}

void Fracture::Renderer::RenderDebug()
{
    ProfilerTimer timer("Render debug");
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    m_DebugMaterial = AssetManager::getMaterial("DebugMaterial");
    m_DebugMaterial->getShader()->use();
    m_DebugMaterial->getShader()->setMat4("projection", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getProjectionMatrix(m_width, m_Height));
    m_DebugMaterial->getShader()->setMat4("view", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getViewMatrix());
    m_DebugMaterial->getShader()->setVec4("Color", glm::vec4(0.7f,0.7f,0.0f,1.0f));
    for (int i = 0; i < m_DebugDraws.size(); i++)
    {
        m_DebugDraws[i]->Render();
    }  
    m_DebugMaterial->getShader()->unbind();
   
}

void Fracture::Renderer::RenderDebugRetained()
{
    ProfilerTimer timer("debug retained");
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
 

    m_DebugMaterial = AssetManager::getMaterial("DebugMaterial");
    m_DebugMaterial->getShader()->use();
    m_DebugMaterial->getShader()->setMat4("projection", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getProjectionMatrix(m_width, m_Height));
    m_DebugMaterial->getShader()->setMat4("view", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getViewMatrix());
    m_DebugMaterial->getShader()->setVec4("Color", glm::vec4(0.7f, 0.7f, 0.0f, 1.0f));
    for (int i = 0; i <m_DebugDrawsRetained.size(); i++)
    {
        m_DebugDrawsRetained[i]->Render();
    }
    m_DebugMaterial->getShader()->unbind();
}

void Fracture::Renderer::EndFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_DebugDraws.clear();
    m_lights.clear();
    glDisable(GL_DEPTH_TEST);
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

        SetupLighting(command.material);
       
        Draw(command);

        command.material->getShader()->unbind();        
        glUseProgram(0);       
    }

    for (auto& command : m_opaqueBucket->GetInstanced())
    {
        std::shared_ptr<Material> material = ComponentManager::GetComponent<RenderComponent>(command->ID)->material;
        material->getShader()->use();
        material->getShader()->setMat4("projection", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getProjectionMatrix(m_width, m_Height));
        material->getShader()->setMat4("view", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getViewMatrix());
        material->getShader()->setVec3("viewPos", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->Position);
        material->getShader()->setMat4("model", ComponentManager::GetComponent<TransformComponent>(command->ID)->GetWorldTransform());

        auto* uniforms = material->GetUniforms();
        for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
        {
            switch (it->second.Type)
            {
            case SHADER_TYPE_BOOL:
                material->getShader()->setBool(it->first, it->second.Bool);
                break;
            case SHADER_TYPE_INT:
                material->getShader()->setInt(it->first, it->second.Int);
                break;
            case SHADER_TYPE_FLOAT:
                material->getShader()->setFloat(it->first, it->second.Float);
                break;
            case SHADER_TYPE_VEC2:
                material->getShader()->setVec2(it->first, it->second.Vec2);
                break;
            case SHADER_TYPE_VEC3:
                material->getShader()->setVec3(it->first, it->second.Vec3);
                break;
            case SHADER_TYPE_VEC4:
                 material->getShader()->setVec4(it->first, it->second.Vec4);
                break;
            case SHADER_TYPE_MAT2:
                material->getShader()->setMat2(it->first, it->second.Mat2);
                break;
            case SHADER_TYPE_MAT3:
                material->getShader()->setMat3(it->first, it->second.Mat3);
                break;
            case SHADER_TYPE_MAT4:
                material->getShader()->setMat4(it->first, it->second.Mat4);
                break;
            default:
                //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
                break;
            }
        }

        std::unordered_map<std::string, UniformValueSampler>* uniformsSamplers = material->GetSamplerUniforms();
        for (auto it = uniformsSamplers->begin(); it != uniformsSamplers->end(); ++it)
        {
            switch (it->second.Type)
            {
            case SHADER_TYPE_SAMPLER2D:
                material->getShader()->setTexture(it->first, it->second.texture, it->second.Unit);
                break;
                //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
                break;
            }
        }

        //for (int i = 0; i < Textures.size(); i++)
        //{
        //    material->getShader()->setTexture(command.Textures[i]->type, command.Textures[i].get(), (int)command.Textures[i]->textureType);
        // }

        SetupLighting(material.get());

        DrawInstancedElement(command);

        material->getShader()->unbind();
        glUseProgram(0);

    }
}

void Fracture::Renderer::Draw(RenderCommand command)
{
    glBindVertexArray(command.VAO);
    glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Fracture::Renderer::DrawInstancedElement(std::shared_ptr<RenderInstancedElementsCommand> command)
{
    glBindVertexArray(command->VAO);
    check_gl_error();
    glDrawElementsInstancedBaseVertexBaseInstance(
        GL_TRIANGLES,
        command->count,
        GL_UNSIGNED_INT, (GLvoid*)(sizeof(uint32_t) * command->firstIndex),
        command->primCount,
        command->baseVertex,
        command->baseInstance);
    check_gl_error();
    glBindVertexArray(0);

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

void Fracture::Renderer::PushInstancedElementsCommand(std::shared_ptr<RenderInstancedElementsCommand> command)
{
    m_opaqueBucket->pushInstancedElementCommand(command);
}

void Fracture::Renderer::PushCommand(std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform)
{
    m_opaqueBucket->pushCommand(mesh,material,transform);
}

void Fracture::Renderer::DrawDebugLine(glm::vec3 start, glm::vec3 end)
{
    m_DebugDraws.push_back(std::make_shared<DebugLine>(start,end));
}

void Fracture::Renderer::DrawDebugLineRetained(glm::vec3 start, glm::vec3 end)
{
    m_DebugDrawsRetained.push_back(std::make_shared<DebugLine>(start, end));
}

void Fracture::Renderer::IsDebugRender(bool debug)
{
    m_isDebugRender = debug;
}

void Fracture::Renderer::AddLight(const std::shared_ptr<ILight>& light)
{
    m_lights.push_back(light);
}

void Fracture::Renderer::SetupLighting(Material* material)
{
    for (int i = 0; i < m_lights.size(); i++)
    {
        switch(m_lights[i]->GetLightType())
        {
            case LightType::Sun:
            {
                material->getShader()->setBool("sunLights[" + std::to_string(i) + "].enabled", true);
                material->getShader()->setVec3("sunLights[" + std::to_string(i) + "].direction", m_lights[i]->GetDirection());
                material->getShader()->setVec3("sunLights[" + std::to_string(i) + "].ambient", m_lights[i]->GetAmbient());
                material->getShader()->setVec3("sunLights[" + std::to_string(i) + "].diffuse", m_lights[i]->GetDiffuse());
                material->getShader()->setVec3("sunLights[" + std::to_string(i) + "].specular", m_lights[i]->GetSpecular());
                break;
            }
            case LightType::Point:
            {
                material->getShader()->setBool("pointLights[" + std::to_string(i) + "].enabled", true);
                material->getShader()->setVec3("pointLights[" + std::to_string(i) + "].position", m_lights[i]->GetPosition());
                material->getShader()->setVec3("pointLights[" + std::to_string(i) + "].ambient", m_lights[i]->GetAmbient());
                material->getShader()->setVec3("pointLights[" + std::to_string(i) + "].diffuse", m_lights[i]->GetDiffuse());
                material->getShader()->setVec3("pointLights[" + std::to_string(i) + "].specular", m_lights[i]->GetSpecular());
                material->getShader()->setFloat("pointLights[" + std::to_string(i) + "].constant", m_lights[i]->GetConstant());
                material->getShader()->setFloat("pointLights[" + std::to_string(i) + "].linear", m_lights[i]->GetLinear());
                material->getShader()->setFloat("pointLights[" + std::to_string(i) + "].quadratic", m_lights[i]->GetQuadratic());
                break;
            }
            case LightType::Spot:
            {
                material->getShader()->setBool("spotLights[" + std::to_string(i) + "].enabled", true);
                material->getShader()->setVec3("spotLights[" + std::to_string(i) + "].position", m_lights[i]->GetPosition());
                material->getShader()->setVec3("spotLights[" + std::to_string(i) + "].direction", m_lights[i]->GetDirection());
                material->getShader()->setVec3("spotLights[" + std::to_string(i) + "].ambient", m_lights[i]->GetAmbient());
                material->getShader()->setVec3("spotLights[" + std::to_string(i) + "].diffuse", m_lights[i]->GetDiffuse());
                material->getShader()->setVec3("spotLights[" + std::to_string(i) + "].specular", m_lights[i]->GetSpecular());
                material->getShader()->setFloat("spotLights[" + std::to_string(i) + "].constant", m_lights[i]->GetConstant());
                material->getShader()->setFloat("spotLights[" + std::to_string(i) + "].linear", m_lights[i]->GetLinear());
                material->getShader()->setFloat("spotLights[" + std::to_string(i) + "].quadratic", m_lights[i]->GetQuadratic());
                material->getShader()->setFloat("spotLights[" + std::to_string(i) + "].cutOff", m_lights[i]->GetCutoff());
                material->getShader()->setFloat("spotLights[" + std::to_string(i) + "].outerCutOff", m_lights[i]->GetOuterCutOff());
                break;
            }

        }
       
    }


}

void Fracture::Renderer::RenderEntity(std::shared_ptr<Entity> entity)
{
    ProfilerTimer timer("Render Entity");
    if (!entity)
    {
        return;
    }  
    std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(entity->Id);
    std::shared_ptr<RenderComponent> render = ComponentManager::GetComponent<RenderComponent>(entity->Id);
    std::shared_ptr<LightComponent> lightcomponent = ComponentManager::GetComponent<LightComponent>(entity->Id);
    std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity->Id);  
    if (render && tag->isVisible)
    {      
        for (auto mesh : render->model->GetMeshes())
        {            
            
            if (render->getRenderType() == RenderType::Normal)
            {
                PushCommand(mesh, render->material, transform);
            }
            if (render->getRenderType() == RenderType::InstancedElementsIndirect)
            {               
                for (auto instanceCommand : mesh->GetInstanceCommands())
                {
                    instanceCommand->ID = render->EntityID;
                    PushInstancedElementsCommand(instanceCommand);
                }              
            }
            if (render->getRenderType() == RenderType::InstancedArrayIndirect)
            {
                //PushInstancedArrayCommand(); TODO
            }
        }   
    }
    if (lightcomponent && tag->isVisible)
    {
        AddLight(lightcomponent->GetLight());
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

void Fracture::Renderer::onWindowResize(WindowResizeEvent* mevent)
{
    FRACTURE_TRACE("WINDOW RESIZE");
    setViewport(mevent->Width, mevent->Height);
}



void _check_gl_error(const char* file, int line) {
    GLenum err(glGetError());

    while (err != GL_NO_ERROR) {
        std::string error;

        switch (err) {
        case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
        case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
        case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
        case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
        }

        std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
        err = glGetError();
    }
}