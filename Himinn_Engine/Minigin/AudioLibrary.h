#pragma once
#include "Singleton.h"
#include <string>
#include <vector>

namespace Himinn
{
	class AudioLibrary : public Himinn::Singleton<AudioLibrary>
	{
	public:
		const std::string& GetSound(unsigned char id);
		void AddAudioFile(std::string filepath);
	private:
		std::vector<std::string> m_Sounds;
	};
}