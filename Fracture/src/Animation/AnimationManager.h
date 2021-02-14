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
	class AnimationClip;
	struct AnimationChannel;
	class Skeleton;
	class AnimatorProbe;
	class AnimatorComponent;

	class AnimationManager
	{

	public:
		AnimationManager();
		~AnimationManager();

		void OnUpdate(float dt);

		void BoneTransformation(float dt, std::shared_ptr<AnimatorComponent>& skeleton, std::vector<glm::mat4>& Transforms);
	private:
		AnimatorProbe* m_probe;

		void CalcInterpolatedScaling(glm::vec3& out, const AnimationChannel& animation, const float& animationTime);
		void CalcInterpolatedRotation(glm::quat& out, const AnimationChannel& animation, const float& animationTime);
		void CalcInterpolatedPosition(glm::vec3& out, const AnimationChannel& animation, const float& animationTime);
		void CalcInterpolatedColor(glm::vec4& out, const AnimationChannel& animation, const float& animationTime);

		uint32_t FindRotation(const float& time, const AnimationChannel& channel);
		uint32_t FindScale(const float& time, const AnimationChannel& channel);
		uint32_t FindPosition(const float& time, const AnimationChannel& channel);
		uint32_t FindColor(const float& time, const AnimationChannel& channel);
	};


}

#endif