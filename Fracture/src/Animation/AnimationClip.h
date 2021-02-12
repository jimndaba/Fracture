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
	};


	struct AnimationChannel
	{
		std::string Name;
		std::vector<AnimationKeyframe> m_PositionKeys;
		std::vector<AnimationKeyframe> m_ScaleKeys;
		std::vector<AnimationKeyframe> m_Rotation;
	};
	
	class AnimationClip
	{
	public:
		std::string Name;
		int NumberOfFrames;
		float FramesPerSec;
			
		std::vector<AnimationChannel> m_channels;
	};


}


#endif