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
#include "Entity/Entity.h"
#include "Scene/InstancePool.h"
#include "Scene/Scene.h"
#include "AssetManager/AssetManager.h"
#include "Entity/Camera.h"

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
}

Fracture::Renderer::~Renderer()
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
    Submit();
}

void Fracture::Renderer::EndFrame()
{
    
}

void Fracture::Renderer::Submit()
{
    for(const auto& command : m_opaqueBucket->getCommands())
    {
        std::shared_ptr<Material> material = command.material;
        //set material settings
        
        material->getShader()->use();
        material->getShader()->setMat4("projection", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getProjectionMatrix(&m_window));
        material->getShader()->setMat4("view", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->getViewMatrix());
        material->getShader()->setVec3("viewPos", ComponentManager::GetComponent<CameraControllerComponent>(Scene::MainCamera()->Id)->Position);

        // set uniform state of material
        std::unordered_map<std::string, UniformValue>* uniforms = command.material->GetUniforms();
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

      
        std::vector<std::shared_ptr<Texture>> Textures = command.Textures;
        for (auto it : Textures)
        {
            switch (it->textureType)
            {
            case TextureType::Diffuse:
                material->getShader()->setTexture("material.diffuse",it.get(), (int)it->textureType);
                    break;
            case TextureType::Specular:
                material->getShader()->setTexture("material.specular", it.get(), (int)it->textureType);
                break;
            case TextureType::Normal:
                material->getShader()->setTexture("material.normal", it.get(), (int)it->textureType);
                break;
            case TextureType::Reflection:
                material->getShader()->setTexture("material.reflection", it.get(), (int)it->textureType);
                break;
            case TextureType::Bump:
                material->getShader()->setTexture("material.bump", it.get(), (int)it->textureType);
                break;
            case TextureType::Height:
                material->getShader()->setTexture("material.height", it.get(), (int)it->textureType);
                break;
                //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
                break;
            }
        }

        material->getShader()->setMat4("model", command.transform->GetWorldTransform());

        glBindVertexArray(command.VAO);
        glDrawElements(GL_TRIANGLES, command.indiceSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
  
        for (auto it = uniformsSamplers->begin(); it != uniformsSamplers->end(); ++it)
        {
            switch (it->second.Type)
            {
            case SHADER_TYPE_SAMPLER2D:
                //material->getShader()->setTexture(it->first, it->second.texture, it->second.Unit);
                glActiveTexture(GL_TEXTURE0 + it->second.Unit);
                glBindTexture(GL_TEXTURE_2D, 0);
                break;
                //Log::Message("Unrecognized Uniform type set.", LOG_ERROR);
                break;
            }
        }
     
        for (int  i =0; i < Textures.size();i++)
        {
            glActiveTexture(GL_TEXTURE0 + (int)Textures[i]->textureType);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

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

void Fracture::Renderer::PushInstancedCommand(RenderInstancedCommand command)
{
    m_opaqueBucket->pushInstancedCommand(command);
}

void Fracture::Renderer::PushCommand(std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform)
{
    m_opaqueBucket->pushCommand(mesh,material,transform);
    //push to Shadow Passes
    //push to  Depth Pass
    //push to prost processing
}

void Fracture::Renderer::AddDirectLight(std::shared_ptr<Fracture::DirectLightComponent> directLight)
{
    m_directLights.push_back(directLight);
}

void Fracture::Renderer::AddPointLight(std::shared_ptr<Fracture::PointLightComponent> pointLight)
{
    m_pointLights.push_back(pointLight);
}

void Fracture::Renderer::AddSpotLight(std::shared_ptr<Fracture::SpotLightComponent> spotLight)
{
    m_spotLights.push_back(spotLight);
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
            PushCommand(mesh, render->material, transform);
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



