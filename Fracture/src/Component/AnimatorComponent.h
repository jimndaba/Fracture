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
		AnimatorComponent(uint32_t entityID);
		~AnimatorComponent() = default;

		AnimatorComponent(const AnimatorComponent& animator) :Component(animator.EntityID, ComponentType::None)
		{
			m_CurrentAnimation = animator.m_CurrentAnimation;
			m_animations = animator.m_animations;
			m_skeleton = animator.m_skeleton;
			m_Transforms = animator.m_Transforms;
			
		}
		AnimatorComponent(const AnimatorComponent& animator, const uint32_t& entityID) :Component(entityID, ComponentType::None)
		{		
		}

		virtual void onStart();

		void SetAnimation(const std::string& name);

		void Accept(ISceneProbe* visitor) override;
		void Accept(ISceneProbe* visitor,float dt) override;

		std::shared_ptr<AnimatorComponent> clone(uint32_t entityID) const
		{
			return std::shared_ptr<AnimatorComponent>(this->clone_impl(entityID));
		}

		std::vector<glm::mat4> getAnimationTransforms();	
		void resizeTransforms();
		void pushTransform();

		std::shared_ptr<Skeleton> m_skeleton;
		std::map<std::string, std::shared_ptr<AnimationClip>> m_animations;
		std::shared_ptr<AnimationClip> m_CurrentAnimation;
		std::vector<glm::mat4> m_Transforms;

	private:

		virtual AnimatorComponent* clone_impl(uint32_t entityID) const override
		{
			return new AnimatorComponent(*this, entityID);
		}

	};



}

#endif