#pragma once
#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "Rendering/Mesh.h"
#include "AnimationGraph.h"

namespace Fracture
{
	struct AnimationClip;
	struct AnimationTrack;

	struct GlobalPose
	{
		std::vector<glm::mat4> LocalPoses;
		std::vector<glm::mat4> GlobalPoses;
	};

	struct Skeleton
	{
		uint32_t NumBones;
		std::vector<Bone> Bones;
	};

	class AnimationSystem
	{
		static std::unique_ptr<AnimationSystem> mInstance;
	public: 
		AnimationSystem();

		void Init();

		void Update(float dt);

		void UpdatePose(Fracture::UUID entity, AnimationClip* clip, float time);

		bool GetBoneTrack(AnimationClip* clip, const std::string& name, AnimationTrack& outTrack);

		glm::mat4 BoneTransformation(AnimationTrack& outTrack, float time);

		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		glm::mat4 CalcInterpolatedScaling(AnimationTrack& outTrack, const float& animationTime);
		glm::mat4 CalcInterpolatedRotation(AnimationTrack& outTrack, const float& animationTime);
		glm::mat4 CalcInterpolatedPosition(AnimationTrack& outTrack, const float& animationTime);

		uint32_t FindNextScale(const float& time, AnimationTrack& outTrack);
		uint32_t FindNextRotation(const float& time, AnimationTrack& outTrack);
		uint32_t FindNextPosition(const float& time, AnimationTrack& outTrack);
		

		std::unordered_map<UUID, GlobalPose> mGlobalPoses;
		
		std::unordered_map<UUID, Skeleton> mSkeletons;
		bool HasGlobalPose(UUID entity);
		static AnimationSystem* Instance();
		std::map<Fracture::UUID, std::shared_ptr<AnimationGraph>> mGraphs;
	};








}

#endif