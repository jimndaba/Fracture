#pragma once
#ifndef SCENEPROBES_H
#define SCENEPROBES_H

#include "ISceneProbe.h"

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
			std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(component->EntityID);
			for (auto mesh : component->m_model->GetMeshes())
			{
				if (mRenderer.ActiveCamera()->IsBoxInFrustum(mesh->GetAABB()->min, mesh->GetAABB()->max))
				{
					mRenderer.PushCommand(mesh, AssetManager::getMaterial(mesh->MaterialName), transform);
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