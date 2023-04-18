#pragma once
#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "soloud/soloud.h"
#include "soloud/soloud_wav.h"

#include "Assets/AssetRegistries.h"
#include <unordered_map>

namespace Fracture
{

	struct AudioProperties
	{
		float MasterVolume;
	};

	struct AudioChannel
	{
		Fracture::UUID ChannelID;
		std::string Name;
		float Volume;
		float Pan;
		bool Muted;
		bool Soloed;
		bool IsPlaying = false;
		std::shared_ptr<SoLoud::AudioSource> mChannel;
		SoLoud::handle ChannelHandle;
	};

	struct ChannelGroup
	{
		std::string Name;
		float Volume = 1.0f;
		float Pan;
		bool Muted;
		bool Soloed;
		SoLoud::handle Handle;
		std::vector<UUID> mRoutedChannels;
	};

	struct AudioMixer
	{
		std::map<UUID, std::shared_ptr<AudioChannel>> mChannels;
		std::map<UUID, std::shared_ptr<ChannelGroup>> mGroups;
		std::shared_ptr<ChannelGroup> MasterOut;
		float MasterVolume = 1.0f;
		bool Muted;
	};

	class AudioManager
	{
	public:
		AudioManager();

		void OnInit();

		void OnLoadContent();

		void OnUpdate(float dt);

		void OnStart();

		void OnShutdown();

		AudioProperties Properties;
		void RegisterAudioClip(const AudioRegistry& reg);

		void Play(UUID clip);
		void Play(std::shared_ptr<AudioChannel> channel);

		void SetVolume(std::shared_ptr<AudioChannel> channel);
		void SetMasterVolume(float volume);
		void SetGroupVolume(std::shared_ptr<ChannelGroup> group);
		void SetPan(std::shared_ptr<AudioChannel> channel);

		void Stop(UUID clip);
		void Stop(std::shared_ptr<AudioChannel> channel);
		void StopAll();

		std::unordered_map<Fracture::UUID, std::shared_ptr<SoLoud::AudioSource>> mAudioClips;
		std::map<Fracture::UUID, AudioRegistry> mAudioClipRegister;

		std::shared_ptr<AudioMixer> Mixer;

		Fracture::UUID AddChannel(const std::string& name);
		void RemoveChannel(const std::string& name);
		void RemoveChannelbyID(Fracture::UUID id);


		void AddGroup(const std::string& name);


		//Voice Groups

	private:
		SoLoud::Soloud gSoloud;	
	};


}

#endif