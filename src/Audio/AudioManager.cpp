#include "FracturePCH.h"
#include "AudioManager.h"

Fracture::AudioManager::AudioManager()
{
}

void Fracture::AudioManager::OnInit()
{
	gSoloud.init();

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
	gSoloud.addVoiceToGroup(Mixer->MasterOut->Handle, channel->ChannelHandle);
	channel->IsPlaying = true;
}

void Fracture::AudioManager::SetVolume(std::shared_ptr<AudioChannel> channel)
{
	gSoloud.setVolume(channel->ChannelHandle, channel->Volume);
}

void Fracture::AudioManager::SetMasterVolume(float volume)
{
	gSoloud.setGlobalVolume(volume);
}

void Fracture::AudioManager::SetGroupVolume(std::shared_ptr<ChannelGroup> group)
{
	gSoloud.setVolume(group->Handle, group->Volume);
}

void Fracture::AudioManager::SetPan(std::shared_ptr<AudioChannel> channel)
{
	gSoloud.setPan(channel->ChannelHandle, channel->Pan);
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

}
