#pragma once
#ifndef ANIMATIONPOSEPOOL_H
#define ANIMATIONPOSEPOOL_H

#include "Animation/GraphNodes/IAnimationNode.h"
#include "Animation/AnimationClip.h"

namespace Fracture
{

	struct PoseBuffer
	{
		int bufferIndex;
		Fracture::UUID NodeID;
		std::vector<PoseSample> Pose;
	};


	class PoseBufferPool
	{
		int mInitialPoolSize;
		int mBufferIndex;
		std::vector<std::unique_ptr<PoseBuffer>> mBufferPool;
		std::list<int> mAvailableBuffers;
		std::list<int> mInUseBuffers;
	public:
		PoseBufferPool(int poolsize);

		void Init();

		PoseBuffer* GetPoseBuffer();
		PoseBuffer* GetPoseBuffer(int index);
		void ReleasePoseBuffer();
		void ReleaseAllBuffers();
		void ReleasePoseBuffer(int index);
		int MovePoseAndClearBuffer(int buffer, int toIndex);

		bool IsInUse(int Index);
		int GetLastBufferInUse();
		
		void MakeUnavailable(int t, std::queue<int>& q);
	};




}

#endif