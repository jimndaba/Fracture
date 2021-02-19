#pragma once
#ifndef ANIMATIONCLIP_H
#define ANIMATIONCLIP_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>

namespace Fracture
{

	struct AnimationKeyframe
	{
		glm::vec3 Position_key;
		glm::quat Rotation_key;
		glm::vec3 Scale_key;
		glm::vec2 VEC2;
		glm::vec3 VEC3;
		glm::vec4 VEC4;
		double Time;
	};

	struct AnimationChannel
	{
		//Bone Name is also Channel Name
		std::string Name;
		std::vector<AnimationKeyframe> m_PositionKeys;
		std::vector<AnimationKeyframe> m_ScaleKeys;
		std::vector<AnimationKeyframe> m_RotationKeys;
		std::vector<AnimationKeyframe> m_ColorKeys;
	};
	
	class AnimationClip
	{
	public:
		//Name of Animation similar to Name of Bone
		std::string Name = std::string{};
		int NumberOfFrames = 0;
		float FramesPerSec = 0.0f;
		float AnimationTime = 0.0f;
		std::vector<AnimationChannel> m_channels;
	};

}


#endif