#pragma once
#ifndef SCENEPROBES_H
#define SCENEPROBES_H

#include "ISceneProbe.h"
#include "Component/ITransform.h"
#include "Profiling/Profiler.h"
#include "Component/AnimatorComponent.h"

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
				auto mesh = meshes[i];
				auto material = component->GetModel()->m_materials[mesh->MaterialIndex()];

				std::shared_ptr<TransformComponent> m_transformComponent = ComponentManager::GetComponent<TransformComponent>(component->EntityID);
		
				if (mRenderer.ActiveCamera()->IsBoxInFrustum(mesh->GetAABB()->min, mesh->GetAABB()->max))
				{					
					DrawCommand command = DrawCommand{};
					command.VAO = mesh->RenderID();
					command.material = material.get();
					command.CastShadows = material->CastShadows();
					command.HasTransparency = material->IsTransparent();
					command.IsOutlined = material->IsOutlined();
					command.ID = component->EntityID;
					command.indiceSize = mesh->CountOfIndices();
					command.Transform = m_transformComponent->GetWorldTransform();
					command.Color = component->Color;;

					if (ComponentManager::HasComponent<AnimatorComponent>(component->EntityID))
					{
						auto& animator = ComponentManager::GetComponent<AnimatorComponent>(component->EntityID);
						command.AnimationTransforms = animator->getAnimationTransforms();
						command.IsAnimated = true;
					}
					mRenderer.PushCommand(command);
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
			mRenderer.DrawBillboard(component->EntityID,component->GetBillboard(),glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), AssetManager::getTexture2D("LightIcon"));
		}

	private:
		Renderer& mRenderer;
	};
}

#endif