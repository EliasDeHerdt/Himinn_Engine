#pragma once
#include "SoundSystem.h"

using namespace std;
namespace Himinn
{
	class NullSoundSystem final : public ISoundSystem
	{
	public:
		void ToggleMute() override {}
		void SetVolume(float) override {};
		void QueueSound(const unsigned char, const float) override {}
		void QueueMusic(const unsigned char, const float ) override {}
		void Play(bool, const unsigned char, const float) override {}
	};
	
	class SoundServiceLocator final
	{
	public:
		~SoundServiceLocator();
		SoundServiceLocator(const SoundServiceLocator&) = delete;
		SoundServiceLocator& operator=(const SoundServiceLocator&) = delete;
		SoundServiceLocator(SoundServiceLocator&&) = delete;
		SoundServiceLocator& operator=(SoundServiceLocator&&) = delete;
		
		static ISoundSystem* GetSoundSystem();
		static void RegisterSoundSystem(ISoundSystem* ss);

	private:
		static ISoundSystem* m_pSSInstance;
		static NullSoundSystem m_DefaultSS;
	};
}
