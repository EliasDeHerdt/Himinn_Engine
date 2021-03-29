#pragma once
#include "SoundSystem.h"
#include <queue>
#include <mutex>
#include <thread>

using namespace std;
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
		
		mutex m_Mutex = {};
		condition_variable cv = {};
		queue<SoundInfo> m_SoundQueue = {};

		bool m_KeepThreadAlive = true;
		thread m_AudioThread;
	};
}