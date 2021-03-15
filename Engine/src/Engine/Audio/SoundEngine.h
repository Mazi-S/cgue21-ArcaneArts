#pragma once

#include <irrKlang.h>

namespace Engine {

	class SoundEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static irrklang::ISoundEngine* Get() { return s_SoundEngine; }
	private:
		static irrklang::ISoundEngine* s_SoundEngine;
	};
}