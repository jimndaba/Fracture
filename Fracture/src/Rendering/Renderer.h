#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include <GLAD/glad.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Game/GameWindow.h"
#include "Event/Event.h"
#include "Event/WindowEvents.h"

namespace Fracture
{
	class RenderBucket;
	class RenderCommand;
	class TransformComponent;
	class LightComponent;
	class ILight;
	class Mesh;
	class Shader;
	class Material;
	class Entity;
	class EntityInstance;
	class Scene;
	class RenderTarget;
	class DebugLine;
	struct UniformValue;
	struct UniformValueSampler;
	class ShadowPass;
	class Grid;

	class Renderer
	{
	public:
		Renderer(int width,int height);
		~Renderer();	

		void onInit();

		void BeginFrame(std::shared_ptr<Scene> scene);
		void RenderPasses();
		void RenderDebug();
		void RenderDebugRetained();				
		void Submit(RenderCommand command);
		void WriteUniformData(Shader shader, std::string name, UniformValue value);
		void WriteUniformSampler(Shader shader, std::string name, std::shared_ptr<UniformValueSampler> value);
		void Draw(RenderCommand command);
		void EndFrame();

		void clear();
		void clearColor(float r, float g, float b);
		void setViewport(int width,int height);
		
		void PushCommand(RenderCommand command);
		void PushCommand(std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform);

		static void DrawDebugLine(glm::vec3 start, glm::vec3 end, glm::vec4 color);
		static void DrawDebugLineRetained(glm::vec3 start, glm::vec3 end,  glm::vec4 color);
		static bool IsDebugDraw()
		{
			return m_isDebugRender;
		}

		static bool IsDrawGrid(bool drawgrid)
		{
			m_drawgrid = drawgrid;
		}

		static void SetDebugRender(bool debug);

		void AddLight(const std::shared_ptr<ILight> light);
		void SetupLighting(Material* material);

		void RenderEntity(std::shared_ptr<Entity> entity);		
		void RenderScene(std::shared_ptr<Scene> scene);
			

		void onWindowResize(WindowResizeEvent* mevent);


		std::shared_ptr<RenderTarget> SceneRenderTarget;
		

	private:
		int m_width;
		int m_Height;
		static bool m_isDebugRender;
		static bool m_drawgrid;

		//render buckets
		std::shared_ptr<RenderBucket> m_opaqueBucket;
		std::shared_ptr<RenderBucket> m_transparentBucket;
		std::shared_ptr<RenderBucket> m_shadowBucket;
		
		//Light  Bucket
		std::vector<std::shared_ptr<Fracture::ILight>> m_lights;

		//Debug Draw bucket
		static std::vector<std::shared_ptr<DebugLine>> m_DebugDraws;
		static std::vector<std::shared_ptr<DebugLine>> m_DebugDrawsRetained;

		//Render Passes
		std::shared_ptr<ShadowPass> m_ShadowPass;

		std::shared_ptr<Material> m_DebugMaterial;
		std::shared_ptr<Grid> m_grid;
	};

}

#endif