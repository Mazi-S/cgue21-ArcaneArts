#pragma once

#include <irrKlang.h>
#include <glm/glm.hpp>

namespace Engine {

	class SoundEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static irrklang::ISound* Play2D(irrklang::ISoundSource* source, bool playLooped = false, bool startPaused = false, bool track = true);
		static irrklang::ISound* Play3D(irrklang::ISoundSource* source, glm::vec3 pos, bool playLooped = false, bool startPaused = false, bool track = false);

		static void SetListenerPosition(glm::vec3 position, glm::vec3 lookDirection);

		static irrklang::ISoundEngine* Get() { return s_SoundEngine; }
	private:
		static irrklang::ISoundEngine* s_SoundEngine;
	};
}