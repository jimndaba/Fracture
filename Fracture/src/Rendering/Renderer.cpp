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
#include "Component/ICamera.h"

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
#include "ShadowPass.h"

#include "BillBoard.h"
#include "Grid.h"

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
bool Fracture::Renderer::m_drawgrid;
std::shared_ptr<Fracture::Renderer> Fracture::Renderer::instance;

Fracture::Renderer::Renderer()
{
   m_width = 1280;
   m_Height = 720;
   m_opaqueBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
   m_transparentBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
   m_shadowBucket = std::shared_ptr<RenderBucket>(new RenderBucket());    
   SceneRenderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(1280, 720, GL_FLOAT, 1, true));   
   m_grid = std::make_shared<Grid>(100, 100, 1, 1,0.5f);
   m_grid->SetColor(glm::vec4(0.50f, 0.50f, 0.50f,2.0f));
   DrawDebugLineRetained(glm::vec3(-50.0f,0.0f,0.0f), glm::vec3(50.0f, 0.0f, 0.0f),glm::vec4(0.0f,0.0f,1.0f,1.0f));
   DrawDebugLineRetained(glm::vec3(0.0f, 0.0f, -50.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  }

Fracture::Renderer::~Renderer()
{
}

void Fracture::Renderer::onInit()
{    
    FRACTURE_INFO("Renderer Init");
    Game::GetEventbus()->Subscribe(this ,& Fracture::Renderer::onWindowResize);

    m_ShadowPass = std::shared_ptr<ShadowPass>(new ShadowPass());
    m_isDebugRender = false;
    m_drawgrid = true;
}

void Fracture::Renderer::BeginFrame(std::shared_ptr<Scene> scene)
{
    ProfilerTimer timer("Begin Frame");

    glEnable(GL_DEPTH_TEST);	
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    //Clear All Buckets
	m_opaqueBucket->clear();
    m_transparentBucket->clear();
    m_shadowBucket->clear();

	//Collect Scene Data
    RenderScene(scene);
}

void Fracture::Renderer::RenderPasses()
{
    ProfilerTimer timer("RenderPass");
  
    m_opaqueBucket->sort();   
    m_shadowBucket->sort();
    m_transparentBucket->sort();

    glCullFace(GL_FRONT);  
    glDisable(GL_CULL_FACE);
    m_ShadowPass->Begin();      
    for (auto light : m_lights)
    {
        if (light->GetLightType() == LightType::Sun)
        {
            m_ShadowPass->Prepare(std::static_pointer_cast<SunLight>(light));            
        }  
    }
    m_ShadowPass->Render(AssetManager::getMaterial("DepthMaterial"),*m_shadowBucket);
    m_ShadowPass->End();
    
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    setViewport(m_width, m_Height);        
    SceneRenderTarget->bind(); 

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, SceneRenderTarget->GetColorTexture(0)->id);

    clearColor(0.08f, 0.07f, 0.16f);
    clear();

    for (const auto& command : m_opaqueBucket->getCommands())
    {       
        Submit(command);
    }   
    for (const auto& command : m_transparentBucket->getCommands())
    {       
       Submit(command);
    }
    if (m_drawgrid)
    {
        m_grid->Draw(AssetManager::getShader("DebugShader"), m_camera->getViewMatrix(), m_camera->getProjectionMatrix(m_width, m_Height));
    }    
    if (m_isDebugRender)
    {
        PhysicsManager::DrawDebug();  
        RenderDebug();
        RenderDebugRetained();
    }       
    SceneRenderTarget->Unbind();  

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
    m_DebugMaterial->getShader()->setMat4("projection",m_camera->getProjectionMatrix(m_width, m_Height));
    m_DebugMaterial->getShader()->setMat4("view", m_camera->getViewMatrix());
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
    glEnable(GL_DEPTH_TEST);
 
    glLineWidth(1.0f);
    m_DebugMaterial = AssetManager::getMaterial("DebugMaterial");
    m_DebugMaterial->getShader()->use();
    m_DebugMaterial->getShader()->setMat4("projection", m_camera->getProjectionMatrix(m_width, m_Height));
    m_DebugMaterial->getShader()->setMat4("view", m_camera->getViewMatrix());
    for (int i = 0; i <m_DebugDrawsRetained.size(); i++)
    {
        m_DebugMaterial->getShader()->setVec4("Color", m_DebugDrawsRetained[i]->GetColor());
        m_DebugDrawsRetained[i]->Render();
    }
    m_DebugMaterial->getShader()->unbind();
}

void Fracture::Renderer::EndFrame()
{
    ProfilerTimer timer("End Frame");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
    m_DebugDraws.clear();
    m_lights.clear();
   
}

void Fracture::Renderer::WriteUniformData(Shader shader,std::string name, UniformValue value)
{
    switch (value.Type)
    {
    case SHADER_TYPE_BOOL:
       shader.setBool(name,value.Bool);
        break;
    case SHADER_TYPE_INT:
        shader.setInt(name, value.Int);
        break;
    case SHADER_TYPE_FLOAT:
        shader.setFloat(name, value.Float);
        break;
    case SHADER_TYPE_VEC2:
        shader.setVec2(name, value.Vec2);
        break;
    case SHADER_TYPE_VEC3:
        shader.setVec3(name, value.Vec3);
        break;
    case SHADER_TYPE_VEC4:
        shader.setVec4(name, value.Vec4);
    case SHADER_TYPE_COLOR3:
        shader.setColor3(name, value.Color3);
    case SHADER_TYPE_COLOR4:
        shader.setColor4(name, value.Color4);
        break;
    case SHADER_TYPE_MAT2:
        shader.setMat2(name, value.Mat2);
        break;
    case SHADER_TYPE_MAT3:
        shader.setMat3(name, value.Mat3);
        break;
    case SHADER_TYPE_MAT4:
        shader.setMat4(name, value.Mat4);
        break;
    default:
        //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
        break;
    }
}

void Fracture::Renderer::WriteUniformSampler(Shader shader, std::string name, std::shared_ptr<UniformValueSampler> value)
{
    switch (value->Type)
    {
    case SHADER_TYPE_SAMPLER2D:
        shader.setTexture(name,value->texture,value->Unit);
        break;
        FRACTURE_ERROR("Unrecognized Uniform type set");
        break;
    }
}

void Fracture::Renderer::Submit(RenderCommand command)
{
    ProfilerTimer timer("Submit");    
    command.material->getShader()->use();

    auto* uniforms = command.material->GetUniforms();
    for (auto it = uniforms->begin(); it != uniforms->end(); ++it)
    {
        WriteUniformData(*command.material->getShader(), it->first, it->second);
    }

    std::unordered_map<std::string, std::shared_ptr<UniformValueSampler>>* uniformsSamplers = command.material->GetSamplerUniforms();
    for (auto it = uniformsSamplers->begin(); it != uniformsSamplers->end(); ++it)
    {  
        WriteUniformSampler(*command.material->getShader(), it->first, it->second);
    }

    for (auto it : command.TextureNames)
    {
        std::shared_ptr<Texture> texture =  AssetManager::getTexture(it);
        command.material->getShader()->setTexture(texture->type, texture.get(), (int)texture->textureType);
    }

    SetupLighting(command.material);

    command.material->getShader()->setMat4("lightSpaceMatrix", m_ShadowPass->GetLightSpaceMatrix());
    command.material->getShader()->setTexture("shadowMap", m_ShadowPass->GetRenderTarget()->GetDepthStencilTexture().get(),(int)m_ShadowPass->GetRenderTarget()->GetDepthStencilTexture()->textureType);
    command.material->getShader()->setMat4("view", m_camera->getViewMatrix());   
    command.material->getShader()->setMat4("projection", m_camera->getProjectionMatrix(m_width, m_Height));
    command.material->getShader()->setVec3("viewPos", m_camera->getPosition());
    command.material->getShader()->setMat4("model", ComponentManager::GetComponent<TransformComponent>(command.ID)->GetWorldTransform());

    Draw(command);
   
    command.material->getShader()->unbind(); 
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Fracture::Renderer::Draw(RenderCommand command)
{
    glBindVertexArray(command.VAO);
    glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
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
    m_width = width;
    m_Height = height;
}

void Fracture::Renderer::PushCommand(RenderCommand command)
{
    if (command.HasTransparency)
    {
        m_transparentBucket->pushCommand(command);
    }
    
    if(!command.HasTransparency)
    {
        m_opaqueBucket->pushCommand(command);
    }

    if (command.material->CastShadows())
    {
        m_shadowBucket->pushCommand(command);
    }   
}

void Fracture::Renderer::PushCommand(std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform)
{
    if (material->IsTransparent())
    {
        m_transparentBucket->pushCommand(mesh, material, transform);
    }
    else
    {
        m_opaqueBucket->pushCommand(mesh, material, transform);
    }

    if (material->CastShadows())
    {
        m_shadowBucket->pushCommand(mesh, material, transform);
    }
   
}

void Fracture::Renderer::DrawDebugLine(glm::vec3 start, glm::vec3 end, glm::vec4 color)
{
    m_DebugDraws.push_back(std::make_shared<DebugLine>(start,end,color));
}

void Fracture::Renderer::DrawDebugLineRetained(glm::vec3 start, glm::vec3 end, glm::vec4 color)
{
    m_DebugDrawsRetained.push_back(std::make_shared<DebugLine>(start, end,color));
}

void Fracture::Renderer::DrawBillboard(int id, std::shared_ptr<Texture> texture)
{
    std::shared_ptr<Material> billbaordMaterial = AssetManager::getMaterial("billboardIcons");
    std::shared_ptr<Billboard> billboard = std::shared_ptr<Billboard>(new Billboard());
    RenderCommand command(billbaordMaterial.get());
    command.VAO = billboard->VAO();
    command.ID = id;
    command.HasTransparency = true;
    command.indiceSize = billboard->Indicies();
    billbaordMaterial->SetTexture("IconTexture", texture, 0);
    Renderer::getInstance()->PushCommand(command);
    
}

void Fracture::Renderer::SetDebugRender(bool debug)
{
    m_isDebugRender = debug;
}

void Fracture::Renderer::SetDrawGrid(bool value)
{
    m_drawgrid = value;
}

void Fracture::Renderer::AddLight(const std::shared_ptr<ILight> light)
{
    m_lights.push_back(light);
}

void Fracture::Renderer::SetupLighting(Material* material)
{
    material->getShader()->use();
    material->getShader()->setBool("sunLights[0].enabled", false);
    material->getShader()->setBool("sunLights[1].enabled", false);

    for (int i = 0; i < 10; i++)
    {
        material->getShader()->setBool("pointLights[" + std::to_string(i) + "].enabled", false);
        material->getShader()->setBool("spotLights[" + std::to_string(i) + "].enabled", false);
    }


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

    std::shared_ptr<TagComponent> tag = ComponentManager::GetComponent<TagComponent>(entity->Id);
    std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(entity->Id);
    std::shared_ptr<RenderComponent> render = ComponentManager::GetComponent<RenderComponent>(entity->Id);
    std::shared_ptr<LightComponent> lightcomponent = ComponentManager::GetComponent<LightComponent>(entity->Id);
   
    if (render && tag->isVisible)
    {      
        for (auto mesh : render->model->GetMeshes())
        {            
            PushCommand(mesh, render->material, transform);          
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

void Fracture::Renderer::SetCamera(std::shared_ptr<ICamera> camera)
{
    m_camera = camera;
}

std::shared_ptr<Fracture::ICamera> Fracture::Renderer::ActiveCamera()
{
    return m_camera;
}

void Fracture::Renderer::onWindowResize(WindowResizeEvent* mevent)
{
    FRACTURE_TRACE("WINDOW RESIZE");
    setViewport(mevent->Width, mevent->Height);
}

std::shared_ptr<Fracture::Renderer> Fracture::Renderer::getInstance()
{
    if (instance == 0)
    {
        FRACTURE_ERROR("RENDERER HAS NOT BEEN INITIALISED");
        instance = std::make_shared<Renderer>();
    }
    return instance;
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