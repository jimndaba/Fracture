#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <GLAD/glad.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Fracture
{
	class RenderBucket;
	class RenderCommand;
	class Entity;
	class Scene;
	class Camera;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void update();

		void BeginFrame(std::shared_ptr<Scene> scene);

		void RenderPasses();
		/*
		--Run Render passes:
		--skybox pass
		--light pass
		--Shadow pass
		--Opaque Pass
		--Decal pass
		--Transparency Pass	
		--Effect pass
		*/
		void EndFrame();

		void flush();

		void clear();
		void clearColor(float r, float g, float b);
		void setViewport(int width,int height);
		
		void PushCommand(RenderCommand command);

		void RenderEntity(std::shared_ptr<Entity> entity);
		void RenderScene(std::shared_ptr<Scene> scene);

	private:
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<RenderBucket> m_opaqueBucket;
		std::shared_ptr<RenderBucket> m_transparentBucket;
	};

}

#endif