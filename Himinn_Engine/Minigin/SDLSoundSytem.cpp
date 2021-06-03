#include "MiniginPCH.h"
#include "SDLSoundSytem.h"

#include "AudioLibrary.h"
#include "audio.h"

Himinn::SDLSoundSytem::SDLSoundSytem()
	: m_AudioThread(&Himinn::SDLSoundSytem::ThreadUpdate, this)
{
}

Himinn::SDLSoundSytem::~SDLSoundSytem()
{
	m_KeepThreadAlive = false;
	m_Condition.notify_one();
	m_AudioThread.join();
}

void Himinn::SDLSoundSytem::ThreadUpdate()
{
	while (m_KeepThreadAlive)
	{
		// Wait for te list to be filled
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [this] { return (!m_SoundQueue.empty() || !m_KeepThreadAlive); });

		// We now own the lock
		if (!m_SoundQueue.empty())
		{
			SoundInfo& currentSound = m_SoundQueue.front();
			Play(currentSound.IsSoundEffect, currentSound.id, currentSound.volume);
			m_SoundQueue.pop();
		}

		// Unlock again for the next loop
		lock.unlock();
	}
}

// Toggles the volume, not the decorator
void Himinn::SDLSoundSytem::ToggleMute()
{
	m_Muted = !m_Muted;
}

void Himinn::SDLSoundSytem::SetVolume(float volume)
{
	float check = volume;
	if (check > 100.f)
		check = 100.f;
	else if (check < 0.f)
		check = 0.f;

	m_Volume = check;
}

void Himinn::SDLSoundSytem::QueueSound(const unsigned char id, const float volume)
{
	m_SoundQueue.push(SoundInfo{ true, id, volume });
	m_Condition.notify_one();
}

void Himinn::SDLSoundSytem::QueueMusic(const unsigned char id, const float volume)
{
	m_SoundQueue.push(SoundInfo{ false, id, volume });
	m_Condition.notify_one();
}

void Himinn::SDLSoundSytem::Play(bool AsSoundEffect, const unsigned char id, const float volume)
{
	float actualVolume = m_Muted ? 0.f : volume * (m_Volume / 100.f);

	if (AsSoundEffect)
		playSound(AudioLibrary::GetInstance().GetSound(id).c_str(), (int)actualVolume);
	else
		playMusic(AudioLibrary::GetInstance().GetSound(id).c_str(), (int)actualVolume);
}
