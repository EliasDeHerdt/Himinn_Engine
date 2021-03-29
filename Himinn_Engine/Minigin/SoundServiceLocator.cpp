#include "MiniginPCH.h"
#include "SoundServiceLocator.h"

Himinn::NullSoundSystem Himinn::SoundServiceLocator::m_DefaultSS;
Himinn::ISoundSystem* Himinn::SoundServiceLocator::m_pSSInstance = &m_DefaultSS;

Himinn::SoundServiceLocator::~SoundServiceLocator()
{
	delete m_pSSInstance;
}

Himinn::ISoundSystem* Himinn::SoundServiceLocator::GetSoundSystem()
{
	return m_pSSInstance;
}

void Himinn::SoundServiceLocator::RegisterSoundSystem(ISoundSystem* ss)
{
	if (m_pSSInstance != &m_DefaultSS)
		delete m_pSSInstance;
	
	m_pSSInstance = (ss == nullptr ? &m_DefaultSS : ss);
}
