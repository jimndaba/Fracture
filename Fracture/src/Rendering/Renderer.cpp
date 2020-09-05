#include "Renderer.h"
#include "RenderCommand.h"
#include "RenderBucket.h"
#include "Component/RenderComponent.h"
#include "Component/TransformComponent.h"
#include "Component/ComponentManager.h"
#include "Entity/Entity.h"
#include "Scene/Scene.h"

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


Fracture::Renderer::Renderer()
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
}

void Fracture::Renderer::EndFrame()
{
	//anything to do before flush
	flush();
}

void Fracture::Renderer::flush()
{
	//flush to GPU
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
        RenderCommand command; 
        command.render = render;
        command.transform = transform;
        PushCommand(command);

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
    RenderEntity(scene->Root);
}

