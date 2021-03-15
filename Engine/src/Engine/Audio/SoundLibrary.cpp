#include "egpch.h"
#include "SoundLibrary.h"
#include "SoundEngine.h"

#include <irrKlang.h>

namespace Engine {

	std::unordered_map<std::string, irrklang::ISoundSource*> SoundLibrary::s_Sounds;

	void SoundLibrary::Add(const std::string& name, irrklang::ISoundSource* sound)
	{
		ASSERT(!Exists(name), "Sound already exists!");
		s_Sounds[name] = sound;
	}

	irrklang::ISoundSource* SoundLibrary::Load(const std::string& name, const std::string& filepath, float volume)
	{
		auto sound = SoundEngine::Get()->addSoundSourceFromFile(filepath.c_str());
		sound->setDefaultVolume(volume);
		Add(name, sound);
		return sound;
	}

	irrklang::ISoundSource* SoundLibrary::Get(const std::string& name)
	{
		ASSERT(Exists(name), "Sound not found!");
		return s_Sounds[name];
	}

	bool SoundLibrary::Exists(const std::string& name)
	{
		return s_Sounds.find(name) != s_Sounds.end();
	}

}


