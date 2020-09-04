#include "Renderer.h"


Fracture::Renderer::Renderer()
{
}

Fracture::Renderer::~Renderer()
{
}


void Fracture::Renderer::BeginFrame()
{
}

void Fracture::Renderer::EndFrame()
{
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

