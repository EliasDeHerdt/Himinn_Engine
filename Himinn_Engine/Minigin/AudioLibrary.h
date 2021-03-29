#pragma once
#include "Singleton.h"
#include "SDL_audio.h"

using namespace std;
namespace Himinn
{
	class AudioLibrary : public Himinn::Singleton<AudioLibrary>
	{
	public:
		const string& GetSound(unsigned char id);
		void AddAudioFile(string filepath);
	private:
		vector<string> m_Sounds;
	};
}