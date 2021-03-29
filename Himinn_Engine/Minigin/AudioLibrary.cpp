#include "MiniginPCH.h"
#include "AudioLibrary.h"



const string& Himinn::AudioLibrary::GetSound(unsigned char id)
{
	return m_Sounds.at(id);
}

// Reference for this function found at :
// https://docs.huihoo.com/sdl/sdl-1.0-intro-en/usingsound.html
void Himinn::AudioLibrary::AddAudioFile(string filepath)
{
	m_Sounds.push_back(filepath);
}