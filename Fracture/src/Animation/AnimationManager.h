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

		void AnimateTransform(float dt,const std::shared_ptr<AnimatorComponent>& animator, std::shared_ptr<TransformComponent>& transform);
		void AnimateRenderer(float dt,const std::shared_ptr<AnimatorComponent>& animator, std::shared_ptr<RenderComponent>& renderer);
		void BoneTransformation(float dt,const std::shared_ptr<AnimatorComponent>& animator, std::vector<glm::mat4>& Transforms);
	private:
		AnimatorProbe* m_probe;

		void CalcInterpolatedScaling(glm::vec3& out, const AnimationChannel& animation, const float& animationTime);
		void CalcInterpolatedRotation(glm::quat& out, const AnimationChannel& animation, const float& animationTime);
		void CalcInterpolatedPosition(glm::vec3& out, const AnimationChannel& animation, const float& animationTime);

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