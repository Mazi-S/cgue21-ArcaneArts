#pragma once
#include <string>
#include <irrKlang.h>
#include <glm/glm.hpp>

namespace Engine {
	class SoundLibrary;
}

namespace Engine::Audio {

	class SoundSource
	{
		friend class SoundLibrary;

	public:
		SoundSource(std::string name, std::string filepath, float volume = 1.0f);
		~SoundSource();

		void Init();
		void Destroy();

		irrklang::ISound* Play2D(bool playLooped = false, bool startPaused = false, bool track = true);
		irrklang::ISound* Play3D(glm::vec3 pos, bool playLooped = false, bool startPaused = false, bool track = false);

		std::string GetName() { return m_Name; }
		std::string GetPath() { return m_Path; }

		float GetVolume() { return m_Volume; }
		void SetVolume(float volume);

	private:
		irrklang::ISoundSource* GetSource() { return m_SoundSource; }

	private:
		std::string m_Name;
		std::string m_Path;
		float m_Volume;

		irrklang::ISoundSource* m_SoundSource;
	};

}