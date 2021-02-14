#pragma once
#ifndef ANIMATORPROBE_H
#define ANIMATORPROBE_H


#include "Rendering/ISceneProbe.h"
#include "Profiling/Profiler.h"

namespace Fracture
{
	class AnimationManager;

	class AnimatorProbe :public ISceneProbe
	{
	public:
		AnimatorProbe(AnimationManager& manager);
		void VisitAnimatorComponent(const  std::shared_ptr<AnimatorComponent>& component, float dt) override;
	private:
		AnimationManager& m_manager;
	};

}

#endif