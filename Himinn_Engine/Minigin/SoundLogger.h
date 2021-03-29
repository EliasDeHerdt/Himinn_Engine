#pragma once
#include "SoundSystem.h"

using namespace std;
namespace Himinn
{
	class SoundLogger final : public ISoundSystem
	{
	public:
		SoundLogger(ISoundSystem* ss, bool muted = false);
		~SoundLogger();
		SoundLogger(const SoundLogger&) = delete;
		SoundLogger& operator=(const SoundLogger&) = delete;
		SoundLogger(SoundLogger&&) = delete;
		SoundLogger& operator=(SoundLogger&&) = delete;

		void ToggleMute() override;
		void SetVolume(float volume) override;
		void QueueSound(const unsigned char id, const float volume) override;
		void QueueMusic(const unsigned char id, const float volume) override;
		void Play(bool AsSoundEffect, const unsigned char id, const float volume) override;

	private:
		ISoundSystem* m_pRealSS;
		bool muted;
	};

}