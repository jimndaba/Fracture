#pragma once
#ifndef ANIMATORCOMPONENT_H
#define ANIMATORCOMPONENT_H

#include "Component.h"
#include <map>

namespace Fracture
{
	class Mesh;
	class AnimationClip;

	class AnimatorComponent : public Component
	{
	public:
		AnimatorComponent(uint32_t entityID);
		~AnimatorComponent() = default;

		AnimatorComponent(const AnimatorComponent& animator) :Component(animator.EntityID, ComponentType::None),
			m_meshes(animator.m_meshes)
		{
		}
		AnimatorComponent(const AnimatorComponent& animator, const uint32_t& entityID) :Component(entityID, ComponentType::None),
			m_meshes(animator.m_meshes)
		{		
		}

		virtual void onStart();
		void OnUpdate(float dt);

		glm::mat4 BoneTransformation(float dt, std::shared_ptr<Mesh>& mesh, std::vector<glm::mat4>& Transforms);




		void Accept(ISceneProbe* visitor) override;
		std::shared_ptr<AnimatorComponent> clone(uint32_t entityID) const
		{
			return std::shared_ptr<AnimatorComponent>(this->clone_impl(entityID));
		}

		std::vector<std::shared_ptr<Mesh>>& m_meshes;
		std::map<std::string, std::shared_ptr<AnimationClip>> m_animations;
		std::shared_ptr<AnimationClip> m_CurrentAnimation;
	private:
		virtual AnimatorComponent* clone_impl(uint32_t entityID) const override
		{
			return new AnimatorComponent(*this, entityID);
		}

		//List of AnimationClips
	};



}

#endif