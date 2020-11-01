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
	class RenderInstancedCommand;
	class TransformComponent;
	class DirectLightComponent;
	class SpotLightComponent;
	class PointLightComponent;
	class Mesh;
	class Material;
	class Entity;
	class EntityInstance;
	class Scene;
	class RenderTarget;
	class DebugLine;

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
		void EndFrame();
		void Submit();

		void Draw(RenderCommand command);
		void DrawInstanced(RenderCommand command);


		void clear();
		void clearColor(float r, float g, float b);
		void setViewport(int width,int height);
		
		void PushCommand(RenderCommand command);
		void PushInstancedCommand(RenderInstancedCommand command);
		void PushCommand(std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform);

		static void DrawDebugLine(glm::vec3 start, glm::vec3 end);
		static void DrawDebugLineRetained(glm::vec3 start, glm::vec3 end);

		void AddDirectLight(std::shared_ptr<Fracture::DirectLightComponent> directLight);
		void AddPointLight(std::shared_ptr<Fracture::PointLightComponent> pointLight);
		void AddSpotLight(std::shared_ptr<Fracture::SpotLightComponent> spotLight);

		void RenderEntity(std::shared_ptr<Entity> entity);		
		void RenderScene(std::shared_ptr<Scene> scene);
		std::shared_ptr<RenderTarget> SceneRenderTarget;

		void onWindowResize(WindowResizeEvent* mevent);

	private:
		int m_width;
		int m_Height;
		std::shared_ptr<RenderBucket> m_opaqueBucket;
		std::shared_ptr<RenderBucket> m_transparentBucket;

		std::vector<std::shared_ptr<Fracture::DirectLightComponent>> m_directLights;
		std::vector<std::shared_ptr<Fracture::PointLightComponent>> m_pointLights;
		std::vector<std::shared_ptr<Fracture::SpotLightComponent>> m_spotLights;
		static std::vector<std::shared_ptr<DebugLine>> m_DebugDraws;
		static std::vector<std::shared_ptr<DebugLine>> m_DebugDrawsRetained;
		std::shared_ptr<Material> m_DebugMaterial;
	};

}

#endif