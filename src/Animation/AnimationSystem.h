#pragma once
#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include "AnimationGraph.h"
#include "AnimationBlender.h"
#include "AnimationPosePool.h"

namespace Fracture
{
	struct AnimationClip;
	struct AnimationTrack;

	
	

	class AnimationSystem
	{
		static std::unique_ptr<AnimationSystem> mInstance;
	public: 
		AnimationSystem();

		void Init();

		void Update(float dt);


		void UpdatePose(Fracture::UUID entity, AnimationClip* clip, float time);

		void Blend(Fracture::UUID grap, BlendFunctionType func, BlendSpaceType space, std::vector<PoseSample> inPose1, std::vector<PoseSample> inPose2, float factor);

		bool GetBoneTrack(AnimationClip* clip, const std::string& name, AnimationTrack& outTrack);

		glm::mat4 BoneTransformation(AnimationTrack& outTrack, float time);

		void SampleAnimation(const StaticMesh* mesh, std::vector<PoseSample>& outSample, Fracture::UUID& clipID, float& time, float dt);

		float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

		glm::mat4 CalcInterpolatedScaling(AnimationTrack& outTrack, const float& animationTime);
		glm::mat4 CalcInterpolatedRotation(AnimationTrack& outTrack, const float& animationTime);
		glm::mat4 CalcInterpolatedPosition(AnimationTrack& outTrack, const float& animationTime);

		glm::vec3 SampleScaling(AnimationTrack& outTrack, const float& animationTime);
		glm::quat SampleRotation(AnimationTrack& outTrack, const float& animationTime);
		glm::vec3 SamplePosition(AnimationTrack& outTrack, const float& animationTime);

		uint32_t FindNextScale(const float& time, AnimationTrack& outTrack);
		uint32_t FindNextRotation(const float& time, AnimationTrack& outTrack);
		uint32_t FindNextPosition(const float& time, AnimationTrack& outTrack);

		void ReloadGraphForAllEntities(UUID graph);
		
		bool HasGlobalPose(UUID entity);
		bool EntityHasGraph(UUID entity);
		bool IsPlaying;
		static AnimationSystem* Instance();

		AnimationBlender mBlender;
		std::unique_ptr<PoseBufferPool> mPool;

		std::map<Fracture::UUID, std::vector<Fracture::UUID>> mEntityToGraphTracker;



		std::map<Fracture::UUID, std::shared_ptr<AnimationGraph>> mGraphs;
		std::map<Fracture::UUID, std::vector<glm::mat4>>  mGlobalPoseMatrices;
	};








}

#endif