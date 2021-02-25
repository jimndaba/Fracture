#pragma once
#ifndef ANIMATORCOMPONENT_H
#define ANIMATORCOMPONENT_H

#include "Component.h"
#include <map>
#include <vector>

namespace Fracture
{
	class Mesh;
	class AnimationClip;
	struct AnimationChannel;
	class Skeleton;

	class AnimatorComponent : public Component
	{
	public:
		AnimatorComponent(UUID id);
		~AnimatorComponent() = default;

		AnimatorComponent(const AnimatorComponent& animator) :Component(animator)
		{
			m_CurrentAnimation = animator.m_CurrentAnimation;
			m_animations = animator.m_animations;
			m_skeleton = animator.m_skeleton;
			m_Transforms = animator.m_Transforms;
			
		}
		AnimatorComponent(const AnimatorComponent& animator, const UUID& entityID) :Component(entityID)
		{		
		}

		virtual void onStart();

		void SetAnimation(const std::string& name);

		void Accept(ISceneProbe* visitor) override;
		void Accept(ISceneProbe* visitor,float dt) override;

		std::shared_ptr<AnimatorComponent> clone(UUID id) const
		{
			return std::shared_ptr<AnimatorComponent>(this->clone_impl(id));
		}

		std::vector<glm::mat4> getAnimationTransforms();	
		void resizeTransforms();
		void pushTransform();

		std::shared_ptr<Skeleton> m_skeleton;
		std::map<std::string, std::shared_ptr<AnimationClip>> m_animations;
		std::shared_ptr<AnimationClip> m_CurrentAnimation;
		std::vector<glm::mat4> m_Transforms;

		json serialise(const std::shared_ptr<ComponentSerialiser>& visitor) override;

	private:

		virtual AnimatorComponent* clone_impl(UUID id) const override
		{
			return new AnimatorComponent(*this, id);
		}

	};



}

#endif