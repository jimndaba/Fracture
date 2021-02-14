#pragma once
#ifndef ISCENEPROBE_H
#define ISCENEPROBE_H

#include <memory>

namespace Fracture
{
	class RenderComponent;
	class LightComponent;
	class BillboardComponent;
	class AnimatorComponent;
	
	class ISceneProbe
	{

	public:
		virtual ~ISceneProbe() {};
		virtual void VisitRenderComponent(const RenderComponent* component) {};
		virtual void VisitLightComponent(LightComponent* component) {};
		virtual void VisitBillboardComponent(BillboardComponent* component) {};
		virtual void VisitAnimatorComponent(const std::shared_ptr<AnimatorComponent>& component,float dt) {};
	};

}
#endif