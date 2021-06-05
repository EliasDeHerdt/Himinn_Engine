#include "MiniginPCH.h"
#include "SoundLogger.h"

Himinn::SoundLogger::SoundLogger(ISoundSystem* ss, bool muted)
	: m_pRealSS(ss)
	, muted(muted)
{
}

Himinn::SoundLogger::~SoundLogger()
{
	delete m_pRealSS;
}

// Toggles the volume, not the decorator
void Himinn::SoundLogger::ToggleMute()
{
	if (!muted)
		m_pRealSS->ToggleMute();
	std::cout << "Toggled mute\n";
}

void Himinn::SoundLogger::SetVolume(float volume)
{
	if (!muted)
		m_pRealSS->SetVolume(volume);
	std::cout << "Set general volume to " << to_string(volume) << '\n';
}

void Himinn::SoundLogger::QueueSound(const unsigned char id, const float volume)
{
	if (!muted)
		m_pRealSS->QueueSound(id, volume);
	std::cout << "Queuing sound " << to_string(id) << " at volume " << volume << '\n';
}

void Himinn::SoundLogger::QueueMusic(const unsigned char id, const float volume)
{
	if (!muted)
		m_pRealSS->QueueMusic(id, volume);
	std::cout << "Queuing music " << to_string(id) << " at volume " << volume << '\n';
}

void Himinn::SoundLogger::Play(bool AsSoundEffect, const unsigned char id, const float volume)
{
	if (!muted)
		m_pRealSS->Play(AsSoundEffect, id, volume);
	std::cout << "Playing " << (AsSoundEffect ? "sound " : "music ") << to_string(id) << " at volume " << volume << '\n';
}
