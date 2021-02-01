#pragma once
#ifndef SCENEPROBES_H
#define SCENEPROBES_H

#include "ISceneProbe.h"
#include "Component/ITransform.h"
#include "Profiling/Profiler.h"
namespace Fracture
{

	class RenderOperation :public ISceneProbe
	{

	public:
		RenderOperation(Renderer& renderer):mRenderer(renderer)
		{
			
		}

		~RenderOperation() 
		{
		};

		virtual void VisitRenderComponent(const RenderComponent* component)
		{
			ProfilerTimer timer("Visit Render Comp");
			std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(component->EntityID);
			std::vector<std::shared_ptr<Mesh>> meshes = component->GetModel()->GetMeshes();
			for (int i = 0 ; i < meshes.size();i++)
			{
				ProfilerTimer timer("Scene probe for each mesh");
				std::shared_ptr<Mesh> mesh = meshes[i];
				std::shared_ptr<Material> material = component->GetModel()->m_materials[mesh->MaterialIndex];

				std::shared_ptr<TransformComponent> m_transformComponent = ComponentManager::GetComponent<TransformComponent>(component->EntityID);
		
				if (mRenderer.ActiveCamera()->IsBoxInFrustum(mesh->GetAABB()->min, mesh->GetAABB()->max))
				{
					mRenderer.PushCommand(component->EntityID, mesh, material, m_transformComponent->GetWorldTransform());
				}		
			}			
		}
	private:
		Renderer& mRenderer;
	};

	class LightOperation :public ISceneProbe
	{
	public:
		LightOperation(Renderer& renderer) :mRenderer(renderer)
		{

		}
		
		~LightOperation() {}

		virtual void VisitLightComponent(LightComponent* component)
		{		
			mRenderer.AddLight(component->GetLight());
		}

	private:
		Renderer& mRenderer;
	};

	class BillboardOperation :public ISceneProbe
	{
	public:
		BillboardOperation(Renderer& renderer) :mRenderer(renderer)
		{

		}

		~BillboardOperation() {}

		virtual void VisitBillboardComponent(BillboardComponent* component)
		{
			mRenderer.DrawBillboard(component->EntityID,component->GetBillboard(),glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), AssetManager::getTexture("LightIcon"));
		}

	private:
		Renderer& mRenderer;
	};
}

#endif