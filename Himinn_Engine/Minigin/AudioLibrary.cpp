#include "MiniginPCH.h"
#include "AudioLibrary.h"

using namespace std;
const string& Himinn::AudioLibrary::GetSound(unsigned char id)
{
	return m_Sounds.at(id);
}

void Himinn::AudioLibrary::AddAudioFile(string filepath)
{
	m_Sounds.push_back(filepath);
}