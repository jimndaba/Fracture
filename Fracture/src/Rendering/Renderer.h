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
	class DrawCommand;
	class TransformComponent;
	class LightComponent;
	class ILight;
	class Mesh;
	struct BoundingBox;
	class Shader;
	class Material;
	class Texture;
	class Entity;
	class EntityInstance;
	class Scene;
	class RenderTarget;
	class DebugLine;
	struct UniformValue;
	struct UniformValueSampler;
	class ShadowPass;
	class PickingPass;
	class Grid;
	class ICamera;
	class Environment;
	class Billboard;
	class RenderOperation;
	class LightOperation;
	class BillboardOperation;

	class Renderer
	{
	public:
		Renderer();
		~Renderer();	

		void onInit();

		void BeginFrame(std::shared_ptr<Scene> scene);
		void RenderEnvironment();
		void RenderDirectLightShadows();
		void RenderPasses();
		void DrawGrid();
		void RenderDebug();
		void RenderDebugRetained();		
		void RenderOutlined();
		void Submit(DrawCommand command);
		void WriteUniformData(Shader shader, std::string name, UniformValue value);
		void WriteUniformSampler(Shader shader, std::string name, std::shared_ptr<UniformValueSampler> value);
		void Draw(DrawCommand command);
		void EndFrame();

		void SetPickingPass(PickingPass* pass);

		void clear();
		void clearColor(float r, float g, float b);
		void setViewport(int width,int height);
		
		void PushCommand(DrawCommand command);
		void PushCommand(std::shared_ptr<Fracture::Mesh> mesh, std::shared_ptr<Fracture::Material> material, std::shared_ptr<Fracture::TransformComponent> transform);

		static void DrawDebugLine(glm::vec3 start, glm::vec3 end, glm::vec4 color);
		static void DrawDebugLineRetained(glm::vec3 start, glm::vec3 end,  glm::vec4 color);
		static void DrawBillboard(int id, std::shared_ptr<Billboard> billboard, glm::vec4 color, std::shared_ptr<Texture> texture);
		static void DrawAABB(const BoundingBox& aabb, const glm::mat4& transform, const glm::vec4& color);

		static bool IsDebugDraw()
		{
			return m_isDebugRender;
		}
		static bool IsDrawGrid()
		{
			return m_drawgrid;
		}
		static void SetDebugRender(bool debug);
		static void SetDrawGrid(bool value);

		void AddLight(const std::shared_ptr<ILight> light);
		void SetupLighting(Material* material);

		
		void RenderScene(std::shared_ptr<Scene> scene);

		void SetCamera(std::shared_ptr<ICamera> camera);
		static std::shared_ptr<ICamera> ActiveCamera();
			

		void onWindowResize(WindowResizeEvent* mevent);

		//DepthPass
		int NumberDraw;
		int NumberBatches;


		//Shadows
		std::shared_ptr<ShadowPass> m_ShadowPass;

		//Picking
		PickingPass* m_PickingPass;

		//Scene 
		std::shared_ptr<RenderTarget> SceneRenderTarget;

		//render buckets
		std::shared_ptr<RenderBucket> m_opaqueBucket;
		std::shared_ptr<RenderBucket> m_transparentBucket;
		std::shared_ptr<RenderBucket> m_shadowBucket;
		std::shared_ptr<RenderBucket> m_outlineBucket;

	
		
		static std::shared_ptr<Renderer> getInstance();

		uint32_t GetEntityID(int mouseX, int mouseY);

		int Width()
		{
			return m_width;
		}

		int Height()
		{
			return m_Height;
		}

	private:
		int m_width;
		int m_Height;
		unsigned int uboMatrices;
		static bool m_isDebugRender;
		static bool m_drawgrid;
		static std::shared_ptr<Renderer> instance;		

		//camera
		static std::shared_ptr<ICamera> m_camera;

		
		
		//Light  Bucket
		std::vector<std::shared_ptr<Fracture::ILight>> m_lights;

		//Debug Draw bucket
		static std::vector<std::shared_ptr<DebugLine>> m_DebugDraws;
		static std::vector<std::shared_ptr<DebugLine>> m_DebugDrawsRetained;

		//Probes
		RenderOperation* mRenderProbe;
		LightOperation* mLightProbe;
		BillboardOperation* mBillboardProbe;

		std::shared_ptr<Material> m_DebugMaterial;
		std::shared_ptr<Grid> m_grid;
	};

}

#endif