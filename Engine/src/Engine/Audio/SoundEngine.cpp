#include "egpch.h"
#include "SoundEngine.h"
#include <irrKlang.h>

namespace Engine {

	irrklang::ISoundEngine* SoundEngine::s_SoundEngine;

	void SoundEngine::Init()
	{
		LOG_INFO("SoundEngine::Init...");
		s_SoundEngine = irrklang::createIrrKlangDevice();
	}

	void SoundEngine::Shutdown()
	{
		LOG_INFO("SoundEngine::Shutdown...");
		s_SoundEngine->drop();
		s_SoundEngine = nullptr;
	}

	irrklang::ISound* SoundEngine::Play2D(irrklang::ISoundSource* source, bool playLooped, bool startPaused, bool track)
	{
		return s_SoundEngine->play2D(source, playLooped, startPaused, track);
	}

	irrklang::ISound* SoundEngine::Play3D(irrklang::ISoundSource* source, glm::vec3 pos, bool playLooped, bool startPaused, bool track)
	{
		return s_SoundEngine->play3D(source, {pos.x, pos.y, pos.z}, playLooped, startPaused, track);
	}

	void SoundEngine::SetListenerPosition(glm::vec3 position, glm::vec3 lookDirection)
	{
		s_SoundEngine->setListenerPosition({ position.x, position.y, position.z }, { lookDirection.x, lookDirection.y, lookDirection.z });
	}

	irrklang::ISoundSource* SoundEngine::LoadSoundSource(const std::string& filepath)
	{
		return s_SoundEngine->getSoundSource(filepath.c_str(), true);
	}

	void SoundEngine::RemoveSoundSource(irrklang::ISoundSource* source)
	{
		if (s_SoundEngine != nullptr)
			s_SoundEngine->removeSoundSource(source);
	}

}
