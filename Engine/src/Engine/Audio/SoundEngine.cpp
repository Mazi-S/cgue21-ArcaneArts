#include "egpch.h"
#include "SoundEngine.h"

#include <irrKlang.h>

namespace Engine {

	irrklang::ISoundEngine* SoundEngine::s_SoundEngine;

	void SoundEngine::Init()
	{
		s_SoundEngine = irrklang::createIrrKlangDevice();
	}

	void SoundEngine::Shutdown()
	{
		s_SoundEngine->drop();
	}

}
