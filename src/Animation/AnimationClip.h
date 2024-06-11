#pragma once
#ifndef ANIMATIONCLIP_H
#define ANIMATIONCLIP_H


namespace Fracture
{
	//QTS Pose
	struct AnimationPose
	{
		glm::quat Rotation = glm::quat(0, 0, 0, 0);
		glm::vec3 Position = glm::vec3(0);	
		glm::vec3 Scale = glm::vec3(1);
		float Time = 0;
	};

	struct AnimationTrack
	{
		int BoneID;
		std::string Name = std::string{};
		std::vector<AnimationPose> Positions;		
		std::vector<AnimationPose> Scales;		
		std::vector<AnimationPose> Rotations;		
		uint32_t mNumPositionKeys = 0;
		uint32_t mNumScaleKeys = 0;
		uint32_t mNumRotationKeys = 0;
	};

	struct AnimationClip
	{
		std::string Name = std::string{};
		int NumberOfFrames = 0;
		float Duration = 0;
		int FramesPerSec = 0.0f;
		float AnimationTime = 0.0f;
		float CurrentFrame = 0.0f;
		std::vector<AnimationTrack> AnimationTracks;
	};


}

#endif