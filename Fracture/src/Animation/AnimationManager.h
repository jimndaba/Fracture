#pragma once
#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>
#include <glm/gtx/transform.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtx/compatibility.hpp"
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace Fracture
{
	struct AnimationKeyframe;
	class AnimationClip;
	struct AnimationChannel;
	struct Bone;
	class Skeleton;
	class AnimatorProbe;
	class AnimatorComponent;
	class TransformComponent;
	class RenderComponent;

	class AnimationManager
	{

	public:
		AnimationManager();
		~AnimationManager();

		void OnUpdate(float dt);

		void AnimateTransform(float dt,AnimatorComponent* animator, std::shared_ptr<TransformComponent>& transform);
		void AnimateRenderer(float dt,AnimatorComponent* animator, std::shared_ptr<RenderComponent>& renderer);
		void BoneTransformation(const AnimationChannel& channel, const  std::shared_ptr<Skeleton>& skeleton, const float& time);
		void UpdateFinalTransforms(const std::shared_ptr<Skeleton>& skeleton);
		void UpdateHierachy(const std::shared_ptr<Bone>& bone, const std::shared_ptr<Skeleton>& skeleton, const glm::mat4& parentTransform);
		void PushTransforms(AnimatorComponent* animator);


	private:
		AnimatorProbe* m_probe;
	
		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		glm::mat4 CalcInterpolatedScaling(const AnimationChannel& animation, const float& animationTime);
		glm::mat4 CalcInterpolatedRotation(const AnimationChannel& animation, const float& animationTime);
		glm::mat4 CalcInterpolatedPosition(const AnimationChannel& animation, const float& animationTime);

		void CalcInterpolatedvec2(glm::vec2& out, const std::vector<AnimationKeyframe>& keyframes, const float& animationTime);
		void CalcInterpolatedvec3(glm::vec3& out, const std::vector<AnimationKeyframe>& keyframes, const float& animationTime);
		void CalcInterpolatedvec4(glm::vec4& out, const std::vector<AnimationKeyframe>& keyframes, const float& animationTime);

		uint32_t FindRotation(const float& time, const AnimationChannel& channel);
		uint32_t FindScale(const float& time, const AnimationChannel& channel);
		uint32_t FindPosition(const float& time, const AnimationChannel& channel);

		uint32_t FindNextKeyFrame(const float& time, const std::vector<AnimationKeyframe>& keyframes);
	};


}

#endif