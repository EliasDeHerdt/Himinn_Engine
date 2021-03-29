#pragma once

namespace Himinn
{
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;

		// Toggles the volume, not the decorator
		virtual void ToggleMute() = 0;
		virtual void SetVolume(float volume) = 0;
		virtual void QueueSound(const unsigned char id, const float volume) = 0;
		virtual void QueueMusic(const unsigned char id, const float volume) = 0;
		virtual void Play(bool AsSoundEffect, const unsigned char id, const float volume) = 0;
	};
}