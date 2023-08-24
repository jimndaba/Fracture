#include "FracturePCH.h"
#include "AnimationPosePool.h"


Fracture::PoseBufferPool::PoseBufferPool(int poolsize):
	mInitialPoolSize(poolsize)
{
}

void Fracture::PoseBufferPool::Init()
{
	mBufferPool.resize(mInitialPoolSize);
	for (int i = 0; i < mInitialPoolSize; i++)
	{
		mBufferPool[i] = std::make_unique<PoseBuffer>();
		mBufferPool[i]->bufferIndex = i;
		mAvailableBuffers.push_back(i);
	}
}

Fracture::PoseBuffer* Fracture::PoseBufferPool::GetPoseBuffer()
{
	mBufferIndex = mAvailableBuffers.front();

	if (mBufferPool[mBufferIndex])
	{
		mInUseBuffers.push_front(mBufferIndex);
		BuffersCount += 1;
		mAvailableBuffers.pop_front();
		return mBufferPool[mBufferIndex].get();
	}
	return nullptr;
}

Fracture::PoseBuffer* Fracture::PoseBufferPool::GetPoseBuffer(int index)
{	
	if (mBufferPool[index])
	{				
		mBufferIndex = index;
		mAvailableBuffers.remove(index);
		//mInUseBuffers.push_front(index);
		return mBufferPool[index].get();
	}
	return nullptr;
}

void Fracture::PoseBufferPool::ReleasePoseBuffer()
{
	mBufferPool[mBufferIndex]->Pose.clear();
	mAvailableBuffers.push_front(mBufferIndex);
	mInUseBuffers.pop_front();
	BuffersCount -= 1;
}

void Fracture::PoseBufferPool::ReleaseAllBuffers()
{
	BuffersCount = 0;
	while (!mInUseBuffers.empty())
	{
		
		int index = mInUseBuffers.front();
		mBufferPool[index]->Pose.clear();
		mAvailableBuffers.push_front(index);
		mInUseBuffers.pop_front();
		
	}
}

void Fracture::PoseBufferPool::ReleasePoseBuffer(int index)
{
	mBufferPool[index]->Pose.clear();
	mAvailableBuffers.push_front(index);
	mInUseBuffers.remove(index);
	BuffersCount -= 1;
}

int Fracture::PoseBufferPool::MovePoseAndClearBuffer(int buffer, int iIndex)
{		
	int nextAvailable = mAvailableBuffers.front();
	
	mBufferPool[nextAvailable]->Pose.clear();
	mBufferPool[nextAvailable]->Pose.resize(mBufferPool[buffer]->Pose.size());

	mBufferPool[nextAvailable]->bufferIndex = mBufferPool[buffer]->bufferIndex;
	mBufferPool[nextAvailable]->NodeID = mBufferPool[buffer]->NodeID;

	for (int i = 0; i < mBufferPool[buffer]->Pose.size(); i++)
	{
		mBufferPool[nextAvailable]->Pose[i] = mBufferPool[buffer]->Pose[i];

	}
	
	
	mAvailableBuffers.remove(buffer);
	mInUseBuffers.push_front(nextAvailable);
	BuffersCount -= 1;
	ReleasePoseBuffer(buffer);
	return nextAvailable;
}

bool Fracture::PoseBufferPool::IsInUse(int Index)
{
	std::list<int>::iterator itr;
	for (itr = mInUseBuffers.begin();
		itr != mInUseBuffers.end(); itr++)
	{
		return Index == *itr;
	}
	return false;
}

int Fracture::PoseBufferPool::GetLastBufferInUse()
{
	return mInUseBuffers.front();
}

void Fracture::PoseBufferPool::MakeUnavailable(int t, std::queue<int>& q)
{
    mAvailableBuffers.remove(t);    
}
