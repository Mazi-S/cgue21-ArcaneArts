#include "egpch.h"
#include "SoundSource.h"
#include "SoundEngine.h"

namespace Engine::Audio {

	SoundSource::SoundSource(std::string name, std::string filepath, float volume)
		:m_Name(name), m_Path(filepath), m_Volume(volume), m_SoundSource(nullptr)
	{
		Init();
	}

	SoundSource::~SoundSource()
	{
		Destroy();
	}

	void SoundSource::Init()
	{
		m_SoundSource = SoundEngine::LoadSoundSource(m_Path);
		m_SoundSource->setDefaultVolume(m_Volume);
	}

	void SoundSource::Destroy()
	{
		if (m_SoundSource != nullptr)
		{
			SoundEngine::RemoveSoundSource(m_SoundSource);
			m_SoundSource = nullptr;
		}
	}

	irrklang::ISound* SoundSource::Play2D(bool playLooped, bool startPaused, bool track)
	{
		return SoundEngine::Play2D(m_SoundSource, playLooped, startPaused, track);
	}

	irrklang::ISound* SoundSource::Play3D(glm::vec3 pos, bool playLooped, bool startPaused, bool track)
	{
		return SoundEngine::Play3D(m_SoundSource, pos, playLooped, startPaused, track);
	}

	void SoundSource::SetVolume(float volume)
	{
		m_Volume = volume;
		m_SoundSource->setDefaultVolume(m_Volume);
	}

}
