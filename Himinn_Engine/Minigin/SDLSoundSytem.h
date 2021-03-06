#pragma once
#include "SoundSystem.h"
#include <queue>
#include <mutex>
#include <thread>

namespace Himinn
{
	class SDLSoundSytem final : public ISoundSystem
	{
	public:
		SDLSoundSytem();
		~SDLSoundSytem();
		SDLSoundSytem(const SDLSoundSytem&) = delete;
		SDLSoundSytem& operator=(const SDLSoundSytem&) = delete;
		SDLSoundSytem(SDLSoundSytem&&) = delete;
		SDLSoundSytem& operator=(SDLSoundSytem&&) = delete;

		void ThreadUpdate();

		void ToggleMute() override;
		void SetVolume(float volume) override;
		void QueueSound(const unsigned char id, const float volume) override;
		void QueueMusic(const unsigned char id, const float volume) override;
		void Play(bool AsSoundEffect, const unsigned char id, const float volume) override;
	
	private:
		// DataStructs
		struct SoundInfo
		{
			bool IsSoundEffect;
			unsigned char id;
			float volume;
		};
		
		// Variables
		bool m_Muted = false;
		float m_Volume = 100.f;
		
		bool m_KeepThreadAlive = true;
		std::thread m_AudioThread;

		std::mutex m_Mutex = {};
		std::condition_variable m_Condition = {};
		std::queue<SoundInfo> m_SoundQueue = {};
	};
}