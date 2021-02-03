#include "Renderer.h"
#include "DrawCommand.h"
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
#include "Component/ComponentSet.h"
#include "Component/ComponentManager.h"
#include "Component/CameraControllerComponent.h"
#include "Component/TagComponent.h"
#include "Component/ICamera.h"
#include "Component/BillboardComponent.h"

#include "Game/Game.h"
#include "Scene/Scene.h"
#include "RenderTarget.h"
#include "RenderBatch.h"
#include "Entity/Entity.h"
#include "AssetManager/AssetManager.h"
#include "Profiling/Profiler.h"
#include "Logging/Logger.h"
#include "Event/Eventbus.h"
#include "Physics/PhysicsManager.h"
#include "DebugLine.h"
#include "ShadowPass.h"
#include "Framegraph/PassLibrary/PickingPass.h"
#include "Environment.h"
#include "BillBoard.h"
#include "Grid.h"
#include "SceneProbes.h"

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
std::shared_ptr<Fracture::ICamera> Fracture::Renderer::m_camera;
bool Fracture::Renderer::m_isDebugRender;
bool Fracture::Renderer::m_drawgrid;
std::shared_ptr<Fracture::Renderer> Fracture::Renderer::instance;



Fracture::Renderer::Renderer()
{
   m_width = 1920;
   m_Height = 1080;  
}

Fracture::Renderer::~Renderer()
{

}

void Fracture::Renderer::onInit()
{    
    FRACTURE_INFO("Renderer Init");
    Game::GetEventbus()->Subscribe(this ,& Fracture::Renderer::onWindowResize);
    m_Bucket = std::shared_ptr<RenderBucket>(new RenderBucket());
    //m_opaqueBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
    //m_transparentBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
    m_shadowBucket = std::shared_ptr<RenderBucket>(new RenderBucket());
    m_outlineBucket = std::shared_ptr<RenderBucket>(new RenderBucket());

    SceneRenderTarget = std::shared_ptr<RenderTarget>(new RenderTarget(m_width, m_Height, TextureTarget::Texture2D, GL_FLOAT, 1,true));
    m_grid = std::make_shared<Grid>(100, 100, 1, 1, 0.5f);
    m_grid->SetColor(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    //DrawDebugLineRetained(glm::vec3(-50.0f, 0.0f, 0.0f), glm::vec3(50.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    //DrawDebugLineRetained(glm::vec3(0.0f, 0.0f, -50.0f), glm::vec3(0.0f, 0.0f, 50.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));


    m_ShadowPass = std::shared_ptr<ShadowPass>(new ShadowPass());     
    m_PickingPass = std::shared_ptr<PickingPass>(new PickingPass("pickingPass", m_width, m_Height,m_Bucket.get()));

    m_isDebugRender = false;
    m_drawgrid = true;

    mRenderProbe = new RenderOperation(*this);
    mLightProbe = new LightOperation(*this);
    mBillboardProbe = new BillboardOperation(*this);

    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, (3 * sizeof(glm::mat4)) + sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // define the range of the buffer that links to a uniform binding point
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 4 * sizeof(glm::mat4));

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_MULTISAMPLE);
}

void Fracture::Renderer::BeginFrame(std::shared_ptr<Scene> scene)
{
    ProfilerTimer timer("Begin Frame");
    NumberDraw = 0;
    NumberBatches = 0;
   
    glEnable(GL_CULL_FACE);
    
    glDepthFunc(GL_LESS);
    //glEnable(GL_STENCIL_TEST);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_SCISSOR_TEST);

    //Set blending
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Initialize stencil clear value
    //glClearStencil(0);

  
    glCullFace(GL_BACK);
    glDepthMask(GL_TRUE);

    //Clear All Buckets
	//m_opaqueBucket->clear();
    //m_transparentBucket->clear();
    m_Bucket->clear();
    m_shadowBucket->clear();
    m_outlineBucket->clear();

	//Collect Scene Data
    RenderScene(scene);

    m_PickingPass->execute(*this);

   
    //glDisable(GL_CULL_FACE);
    //setViewport(m_width, m_Height);
    //m_PickingPass->Begin();
    //m_PickingPass->Render(m_camera, AssetManager::getMaterial("PickingMaterial"), *m_opaqueBucket);
    //m_PickingPass->Render(m_camera, AssetManager::getMaterial("PickingMaterial"), *m_transparentBucket);
    //m_PickingPass->End();
    //glEnable(GL_CULL_FACE);
}

void Fracture::Renderer::RenderEnvironment()
{
    //setViewport(m_width, m_Height);
    for (auto light : m_lights)
    {
        if (light->GetLightType() == LightType::Sky)
        {
            std::shared_ptr<SkyLight> sky = std::dynamic_pointer_cast<SkyLight>(light);
            sky->GetEnvironment()->Render(AssetManager::getShader("Skybox"), m_camera.get()->getViewMatrix(), m_camera->getProjectionMatrix());
        }
    }

}

void Fracture::Renderer::RenderDirectLightShadows()
{
    ProfilerTimer timer("ShadowPass");
    glDisable(GL_CULL_FACE);
    m_ShadowPass->Begin();
    for (auto light : m_lights)
    {
        if (light->GetLightType() == LightType::Sun && light->CastShadows())
        {
            m_ShadowPass->Prepare(std::static_pointer_cast<SunLight>(light));
        }
    }
    m_ShadowPass->Render(AssetManager::getMaterial("DepthMaterial"), *m_Bucket);
    m_ShadowPass->End();
    glEnable(GL_CULL_FACE);

}

void Fracture::Renderer::RenderPasses()
{
    ProfilerTimer timer("RenderPass");
  
    //m_opaqueBucket->sort();   
    //m_transparentBucket->sort();

    m_Bucket->sort();
    m_shadowBucket->sort();   
    m_outlineBucket->sort();
   
    {
        ProfilerTimer timer("ShadowPass");
        glDisable(GL_CULL_FACE);    
        m_ShadowPass->Begin();    
        for (auto light : m_lights)
        {
            if (light->GetLightType() == LightType::Sun && light->CastShadows())
            {
                m_ShadowPass->Prepare(std::static_pointer_cast<SunLight>(light));            
            }  
        }
        m_ShadowPass->Render(AssetManager::getMaterial("DepthMaterial"),*m_shadowBucket);
        m_ShadowPass->End();
        glEnable(GL_CULL_FACE);
    }

    
    //Picking Pass
    {
        //glDisable(GL_CULL_FACE);
        //setViewport(m_width, m_Height);
        //m_PickingPass->Begin();
        //m_PickingPass->Render(m_camera, AssetManager::getMaterial("PickingMaterial"), *m_opaqueBucket);
        //m_PickingPass->Render(m_camera, AssetManager::getMaterial("PickingMaterial"), *m_transparentBucket);
        //m_PickingPass->End();
        //glEnable(GL_CULL_FACE);
    }
    {
        SceneRenderTarget->bind();    
        setViewport(m_width, m_Height);      
        clearColor(0.10f, 0.10f, 0.10f);
        clear();
        
        if (m_drawgrid)
        {
            m_grid->Draw(AssetManager::getShader("DebugShader"), m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
        }    

        if (m_isDebugRender)
        {
            PhysicsManager::DrawDebug();  
            RenderDebug();
            RenderDebugRetained();
        }     
      
        RenderEnvironment();
        // RenderOutlined();//TODO

        SceneRenderTarget->Unbind();  
    }
}

void Fracture::Renderer::DrawGrid()
{
    if (m_drawgrid)
    {
        m_grid->Draw(AssetManager::getShader("DebugShader"), m_camera->getViewMatrix(), m_camera->getProjectionMatrix());
    }
}

void Fracture::Renderer::RenderDebug()
{
    ProfilerTimer timer("Render debug");
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);   
    glLineWidth(1.0f);
    m_DebugMaterial = AssetManager::getMaterial("DebugMaterial");
    m_DebugMaterial->getShader()->use();
    m_DebugMaterial->getShader()->setMat4("projection",m_camera->getProjectionMatrix());
    m_DebugMaterial->getShader()->setMat4("view", m_camera->getViewMatrix());
    
    for (int i = 0; i < m_DebugDraws.size(); i++)
    {
        m_DebugMaterial->getShader()->setVec4("Color", m_DebugDraws[i]->GetColor());
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
    m_DebugMaterial->getShader()->setMat4("projection", m_camera->getProjectionMatrix());
    m_DebugMaterial->getShader()->setMat4("view", m_camera->getViewMatrix());
    for (int i = 0; i <m_DebugDrawsRetained.size(); i++)
    {
        m_DebugMaterial->getShader()->setVec4("Color", m_DebugDrawsRetained[i]->GetColor());
        m_DebugDrawsRetained[i]->Render();
    }
    m_DebugMaterial->getShader()->unbind();
}

void Fracture::Renderer::RenderOutlined()
{
     
   
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
    case SHADER_TYPE_SAMPLERCUBE:
        shader.setCubeMap(name, value->id, value->Unit);
        break;
        FRACTURE_ERROR("Unrecognized Uniform type set");
        break;
    }
}

void Fracture::Renderer::Submit(DrawCommand command)
{
    ProfilerTimer timer("Submit");
  
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_camera->getProjectionMatrix()));
    glBufferSubData(GL_UNIFORM_BUFFER,  sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_camera->getViewMatrix()));   
    glBufferSubData(GL_UNIFORM_BUFFER, (2 * sizeof(glm::mat4)), sizeof(glm::mat4), glm::value_ptr(m_ShadowPass->GetLightSpaceMatrix()));
    glBufferSubData(GL_UNIFORM_BUFFER, (3 * sizeof(glm::mat4)), sizeof(glm::vec3), glm::value_ptr(m_camera->getPosition()));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    command.material->getShader()->setTexture("shadowMap", m_ShadowPass->GetRenderTarget()->GetDepthStencilTexture().get(), (int)m_ShadowPass->GetRenderTarget()->GetDepthStencilTexture()->textureType);
    command.material->getShader()->setVec3("viewPos", m_camera->getPosition());
    command.material->getShader()->setMat4("model", command.Transform);
    
    Draw(command);
   
}

void Fracture::Renderer::Submit(DrawCommand command,Shader* shader)
{
    ProfilerTimer timer("Submit");

    {
        ProfilerTimer timer("setProjection matries");
        shader->setMat4("projection", m_camera->getProjectionMatrix());
        shader->setMat4("view", m_camera->getViewMatrix());
    }
  

    if (ComponentManager::HasComponent<TransformComponent>(command.ID))
    {
        ProfilerTimer timer("GetTransform");
        shader->setMat4("model", command.Transform);
    }

    Draw(command);

}


void Fracture::Renderer::Draw(DrawCommand command)
{
    ProfilerTimer timer("Draw");
    glBindVertexArray(command.VAO);
    glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Fracture::Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
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
    m_PickingPass->Resize(width, height);
    if (m_camera)
    {
        m_camera->setProjection(width, height);
    }
}

void Fracture::Renderer::PushCommand(DrawCommand command)
{
    NumberDraw += 1;
    if (command.material->CastShadows())
    {
        m_shadowBucket->pushCommand(command);
    }

    m_Bucket->pushCommand(command);
}

void Fracture::Renderer::PushCommand(uint32_t EntityID,std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, glm::mat4 transform)
{
    NumberDraw += 1;
   
    if (material->CastShadows())
    {
        m_shadowBucket->pushCommand(EntityID, mesh, material, transform);
    }

    m_Bucket->pushCommand(EntityID, mesh, material, transform);
       
}

void Fracture::Renderer::DrawDebugLine(glm::vec3 start, glm::vec3 end, glm::vec4 color)
{
    m_DebugDraws.push_back(std::make_shared<DebugLine>(start,end,color));
}

void Fracture::Renderer::DrawDebugLineRetained(glm::vec3 start, glm::vec3 end, glm::vec4 color)
{
    m_DebugDrawsRetained.push_back(std::make_shared<DebugLine>(start, end,color));
}

void Fracture::Renderer::DrawBillboard(int id, std::shared_ptr<Billboard> billboard, glm::vec4 color, std::shared_ptr<Texture> texture)
{
    std::shared_ptr<Material> billbaordMaterial = AssetManager::getMaterial("billboardIcons");   
    billbaordMaterial->setColor4("mColor",color);
    DrawCommand command;
    command.VAO = billboard->VAO();
    command.ID = id;
    command.HasTransparency = true;
    command.indiceSize = billboard->Indicies();
    command.material = billbaordMaterial.get();
    billbaordMaterial->SetTexture("IconTexture", texture, 0);
    billbaordMaterial->setVec3("CameraRight_worldspace",m_camera->Right());
    billbaordMaterial->setVec3("CameraUp_worldspace", m_camera->Up());
    billbaordMaterial->setVec3("BillboardPos",ComponentManager::GetComponent<TransformComponent>(command.ID)->Position());
    billbaordMaterial->setVec2("BillboardSize",glm::vec2(0.5f));
    Renderer::getInstance()->PushCommand(command);
    
}

void Fracture::Renderer::DrawAABB(const BoundingBox& aabb, const glm::mat4& transform, const glm::vec4& color)
{
    glm::vec4 min = { aabb.min.x, aabb.min.y, aabb.min.z, 1.0f };
    glm::vec4 max = { aabb.max.x, aabb.max.y, aabb.max.z, 1.0f };

    glm::vec4 corners[8] =
    {
        transform * glm::vec4 { aabb.min.x, aabb.min.y, aabb.max.z, 1.0f },
        transform * glm::vec4 { aabb.min.x, aabb.max.y, aabb.max.z, 1.0f },
        transform * glm::vec4 { aabb.max.x, aabb.max.y, aabb.max.z, 1.0f },
        transform * glm::vec4 { aabb.max.x, aabb.min.y, aabb.max.z, 1.0f },
                                                 
        transform * glm::vec4 { aabb.min.x, aabb.min.y, aabb.min.z, 1.0f },
        transform * glm::vec4 { aabb.min.x, aabb.max.y, aabb.min.z, 1.0f },
        transform * glm::vec4 { aabb.max.x, aabb.max.y, aabb.min.z, 1.0f },
        transform * glm::vec4 { aabb.max.x, aabb.min.y, aabb.min.z, 1.0f }
    };

    for (uint32_t i = 0; i < 4; i++)
      DrawDebugLine(corners[i], corners[(i + 1) % 4], color);

    for (uint32_t i = 0; i < 4; i++)
       DrawDebugLine(corners[i + 4], corners[((i + 1) % 4) + 4], color);

    for (uint32_t i = 0; i < 4; i++)
       DrawDebugLine(corners[i], corners[i + 4], color);


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
    ProfilerTimer timer("Setup Lighting");
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
                material->getShader()->setFloat("sunLights[" + std::to_string(i) + "].intensity", m_lights[i]->Intensity());
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
                material->getShader()->setFloat("pointLights[" + std::to_string(i) + "].intensity", m_lights[i]->Intensity());
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
                material->getShader()->setFloat("spotLights[" + std::to_string(i) + "].intensity", m_lights[i]->Intensity());
                break;
            }
            case LightType::Sky:
            {
                std::shared_ptr<SkyLight> sky = std::dynamic_pointer_cast<SkyLight>(m_lights[i]);
                material->getShader()->setFloat("intensity", sky->Intensity());
                material->getShader()->setCubeMap("irradianceMap",sky->GetIrradianceMap(),10);
                material->getShader()->setCubeMap("prefilterMap", sky->GetPreFilterMap(), 1);
                material->getShader()->setTexture("brdfLUT",sky->GetBDRFMap().get(), 2);
                break;
            }
        }       
        
    }
}

void Fracture::Renderer::RenderScene(std::shared_ptr<Scene> scene)
{
    ProfilerTimer timer("Render Scene");
    {
        ProfilerTimer timer("Light Probe");
        for (auto component : ComponentManager::GetAllComponents<LightComponent>())
        {
            component->Accept(mLightProbe);
        }
    }
    {
        ProfilerTimer timer("SceneProbe");
        for (auto component : ComponentManager::GetAllComponents<RenderComponent>())
        {
            component->Accept(mRenderProbe);
        }
    }
    {
        ProfilerTimer timer("billboard Probe");
        for (auto component : ComponentManager::GetAllComponents<BillboardComponent>())
        {
            component->Accept(mBillboardProbe);
        }
    }

}

void Fracture::Renderer::SetCamera(std::shared_ptr<ICamera> camera)
{
    m_camera = camera;
    m_camera->setProjection(m_width,m_Height);
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

uint32_t Fracture::Renderer::GetEntityID(int mouseX, int mouseY)
{
    if (m_PickingPass)
    {        
        int Pixel = m_PickingPass->GetPixelInfo(mouseX, mouseY);
        if (Pixel > 0) {
            return  (uint32_t)Pixel;
        }
        return -1;
    }      
    return -1;
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