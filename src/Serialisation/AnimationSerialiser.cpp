#include "FracturePCH.h"
#include "AnimationSerialiser.h"
#include "Animation/AnimationClip.h"

Fracture::AnimationSerialiser::AnimationSerialiser(ISerialiser::IOMode mode, ISerialiser::SerialiseFormat format):
	ISerialiser(mode,format)
{
}

void Fracture::AnimationSerialiser::WriteAnimation(AnimationClip* clip)
{
	BeginStruct("Animation");
	{
		Property("Name", clip->Name);
		Property("Duration", clip->Duration);
		Property("FramesPerSec", clip->FramesPerSec);
		Property("NoOfFrames", clip->NumberOfFrames);

		BeginCollection("Tracks");
		for (const auto& track : clip->AnimationTracks)
		{
			BeginStruct("Track");
			Property("BoneID", track.BoneID);
			Property("Name", track.Name);
			Property("NumPositionKeys", track.mNumPositionKeys);
			Property("NumRotationKeys", track.mNumRotationKeys);
			Property("NumScaleKeys", track.mNumScaleKeys);
			
			BeginCollection("Positions");
			for (const auto& pose : track.Positions)
			{
				BeginStruct("Pose");
				Property("Value", pose.Position);				
				Property("Time", pose.Time);
				EndStruct();
			}
			EndCollection();

			BeginCollection("Rotations");
			for (const auto& pose : track.Rotations)
			{
				BeginStruct("Pose");
				Property("Value", pose.Rotation);
				Property("Time", pose.Time);
				EndStruct();
			}
			EndCollection();

			BeginCollection("Scales");
			for (const auto& pose : track.Scales)
			{
				BeginStruct("Pose");				
				Property("Value", pose.Scale);				
				Property("Time", pose.Time);
				EndStruct();
			}
			EndCollection();

			EndStruct();
		}
		EndCollection();
		
	}
	EndStruct();
}

std::shared_ptr<Fracture::AnimationClip> Fracture::AnimationSerialiser::ReadAnimation()
{
	if (BeginStruct("Animation"))
	{
		auto clip = std::make_shared<AnimationClip>();
		clip->Name = STRING("Name");
		clip->Duration = FLOAT("Duration");
		clip->FramesPerSec = INT("FramesPerSec");
		clip->NumberOfFrames = INT("NoOfFrames");

		if (BeginCollection("Tracks"))
		{
			while (CurrentCollectionIndex() < GetCollectionSize())
			{
				if (BeginStruct("Track"))
				{
					AnimationTrack track{};
					track.BoneID = INT("BoneID");
					track.Name = STRING("Name");
					track.mNumPositionKeys = INT("NumPositionKeys");
					track.mNumScaleKeys = INT("NumScaleKeys");
					track.mNumRotationKeys = INT("NumRotationKeys");

					if (BeginCollection("Positions"))
					{
						while (CurrentCollectionIndex() < GetCollectionSize())
						{
							if (BeginStruct("Pose"))
							{
								AnimationPose pose{};
								pose.Position = VEC3("Value");
								pose.Time = FLOAT("Time");
								track.Positions.push_back(pose);
								EndStruct();
							}
							NextInCollection();
						}
						EndCollection();
					}

					if (BeginCollection("Rotations"))
					{
						while (CurrentCollectionIndex() < GetCollectionSize())
						{
							if (BeginStruct("Pose"))
							{
								AnimationPose pose{};
								pose.Rotation = QUAT("Value");
								pose.Time = FLOAT("Time");
								track.Rotations.push_back(pose);
								EndStruct();
							}
							NextInCollection();
						}
						EndCollection();
					}

					if (BeginCollection("Scales"))
					{
						while (CurrentCollectionIndex() < GetCollectionSize())
						{
							if (BeginStruct("Pose"))
							{
								AnimationPose pose{};
								pose.Scale = VEC3("Value");
								pose.Time = FLOAT("Time");
								track.Scales.push_back(pose);
								EndStruct();
							}
							NextInCollection();
						}
						EndCollection();
					}

					clip->AnimationTracks.push_back(track);
					EndStruct();
				}
				NextInCollection();
			}
			EndCollection();
		}

		EndStruct();
		return clip;
	}




	return std::shared_ptr<AnimationClip>();
}
