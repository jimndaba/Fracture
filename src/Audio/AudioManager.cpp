#include "FracturePCH.h"
#include "AudioManager.h"

Fracture::AudioManager::AudioManager()
{
}

void Fracture::AudioManager::OnInit()
{
	gSoloud.init();
	gSoloud.setVisualizationEnable(true);
	Mixer = std::make_shared<AudioMixer>();
	
	Mixer->MasterOut = std::make_shared<ChannelGroup>();

	Mixer->MasterOut->Handle = gSoloud.createVoiceGroup();
}

void Fracture::AudioManager::OnLoadContent()
{	
	{
		auto c = AddChannel("Theme");
		auto input = std::make_shared<SoLoud::Wav>();
		input->load("Content\\audio\\theme.wav");
		Mixer->mChannels[c]->mChannel = input;
	}
	{
		auto c = AddChannel("Piano");
		auto input = std::make_shared<SoLoud::Wav>();
		input->load("Content\\audio\\piano.wav");
		Mixer->mChannels[c]->mChannel = input;
	}
	{
		AddGroup("CharacterSFX");
		AddGroup("BG");
	}
}

void Fracture::AudioManager::OnUpdate(float dt)
{
}

void Fracture::AudioManager::OnStart()
{
	
}

void Fracture::AudioManager::OnShutdown()
{
	gSoloud.deinit(); // Clean up!
}

void Fracture::AudioManager::RegisterAudioClip(const AudioRegistry& reg)
{
}

void Fracture::AudioManager::Play(UUID clip)
{
	if (mAudioClips.find(clip) == mAudioClips.end())
	{
		return;
	}

	auto audio_clip = mAudioClips[clip];
	gSoloud.play(*audio_clip.get(), 1.0f);
	
}

void Fracture::AudioManager::Play(std::shared_ptr<AudioChannel> channel)
{
	channel->ChannelHandle = gSoloud.play(*channel->mChannel.get(), channel->Volume,channel->Pan);	
	channel->IsPlaying = true;

	
	if (channel->IsGrouped)
	{
		gSoloud.addVoiceToGroup(Mixer->mGroups[channel->GroupID]->Handle, channel->ChannelHandle);
	}
}

void Fracture::AudioManager::SetVolume(std::shared_ptr<AudioChannel> channel)
{
	gSoloud.setVolume(channel->ChannelHandle, channel->Volume);
}

void Fracture::AudioManager::SetMasterVolume(float volume)
{
	gSoloud.setGlobalVolume(volume);
}

float Fracture::AudioManager::GetMasterLevels(int channel)
{
	return gSoloud.getApproximateVolume(channel);
}

void Fracture::AudioManager::SetGroupVolume(std::shared_ptr<ChannelGroup> group)
{
	gSoloud.setVolume(group->Handle, group->Volume);
}

void Fracture::AudioManager::SetPan(std::shared_ptr<AudioChannel> channel)
{
	gSoloud.setPan(channel->ChannelHandle, channel->Pan);
}

void Fracture::AudioManager::SetGroupPan(std::shared_ptr<ChannelGroup> group)
{
	gSoloud.setPan(group->Handle, group->Pan);
}


void Fracture::AudioManager::Stop(UUID clip)
{
	if (mAudioClips.find(clip) == mAudioClips.end())
	{
		return;
	}

	auto audio_clip = mAudioClips[clip];
	gSoloud.stopAudioSource(*audio_clip.get());
}

void Fracture::AudioManager::Stop(std::shared_ptr<AudioChannel> channel)
{
	gSoloud.stopAudioSource(*channel->mChannel.get());
	channel->IsPlaying = false;
}

void Fracture::AudioManager::StopGroup(std::shared_ptr<ChannelGroup> group)
{
	gSoloud.stop(group->Handle);	
	group->IsPlaying = false;

	for (const auto& ids : group->mRoutedChannels)
	{
		Mixer->mChannels[ids]->IsPlaying = false;
	}
}

void Fracture::AudioManager::StopAll() {

	gSoloud.stopAll();	
}

Fracture::UUID Fracture::AudioManager::AddChannel(const std::string& name)
{
	auto channel = std::make_shared<AudioChannel>();
	channel->Name = name;
	Mixer->mChannels[channel->ChannelID] = channel;
	Mixer->MasterOut->mRoutedChannels.push_back(channel->ChannelID);	
	return channel->ChannelID;
}

void Fracture::AudioManager::RemoveChannel(const std::string& name)
{
}

void Fracture::AudioManager::RemoveChannelbyID(Fracture::UUID id)
{
}

void Fracture::AudioManager::AddGroup(const std::string& name)
{
	auto group = std::make_shared < ChannelGroup>();
	group->GroupID = UUID();
	group->Name = name;
	Mixer->mGroups[group->GroupID] = group;
	Mixer->mGroups[group->GroupID]->Handle = gSoloud.createVoiceGroup();
}

void Fracture::AudioManager::RemoveGroup(const std::string& name)
{
}

void Fracture::AudioManager::RemoveGroupByID(Fracture::UUID id)
{
}



void Fracture::AudioManager::AttachChannelToGroup(Fracture::UUID channel_id, Fracture::UUID group_id)
{
	if (Mixer->mChannels.find(channel_id) == Mixer->mChannels.end())
	{
		return;
	}
	if (Mixer->mGroups.find(group_id) == Mixer->mGroups.end())
	{
		return;
	}

	Mixer->mGroups[group_id]->mRoutedChannels.push_back(channel_id);
	
}

void Fracture::AudioManager::DettachChannelFromGroup(Fracture::UUID channel_id, Fracture::UUID group_id)
{
	if (Mixer->mChannels.find(channel_id) == Mixer->mChannels.end())
	{
		return;
	}
	if (Mixer->mGroups.find(group_id) == Mixer->mGroups.end())
	{
		return;
	}
}
