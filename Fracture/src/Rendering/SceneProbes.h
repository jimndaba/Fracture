#pragma once
#ifndef SCENEPROBES_H
#define SCENEPROBES_H

#include "ISceneProbe.h"
#include "Component/ITransform.h"
#include "Profiling/Profiler.h"
#include "Component/AnimatorComponent.h"
#include "Entity/UUID.h"
#include "Component/Component.h"

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
			std::shared_ptr<TransformComponent> transform = ComponentManager::GetComponent<TransformComponent>(component->GetID());
			std::vector<std::shared_ptr<Mesh>> meshes = component->GetModel()->GetMeshes();
			DrawCommand command = DrawCommand{};
			bool isVisible = true;


			if (ComponentManager::HasComponent<AnimatorComponent>(component->GetID()))
			{
				auto& animator = ComponentManager::GetComponent<AnimatorComponent>(component->GetID());
				command.AnimationTransforms = animator->getAnimationTransforms();
				command.IsAnimated = true;
			}

			for (int i = 0 ; i < meshes.size();i++)
			{
				ProfilerTimer timer("Scene probe for each mesh");
				
				isVisible = mRenderer.ActiveCamera()->IsBoxInFrustum(meshes[i]->GetAABB()->min, meshes[i]->GetAABB()->max);				

				if(isVisible)
				{
					auto material = component->GetModel()->m_materials[meshes[i]->MaterialIndex()];
					command.VAO = meshes[i]->RenderID();
					command.ID = component->GetID();
					command.material = material.get();
					command.CastShadows = material->CastShadows();
					command.HasTransparency = material->IsTransparent();
					command.IsOutlined = material->IsOutlined();
					command.indiceSize = meshes[i]->CountOfIndices();
					command.Transform = transform->GetLocalTranform();
					command.Color = component->Color;
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
			mRenderer.DrawBillboard(component->GetID(),component->GetBillboard(),glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), AssetManager::getTexture2D("LightIcon"));
		}

	private:
		Renderer& mRenderer;
	};
}

#endif